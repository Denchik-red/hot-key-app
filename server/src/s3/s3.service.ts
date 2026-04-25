import { Injectable, NotFoundException } from '@nestjs/common';
import {
  S3Client,
  PutObjectCommand,
  GetObjectCommand,
} from '@aws-sdk/client-s3';
import { getSignedUrl } from '@aws-sdk/s3-request-presigner';
import { Multer } from 'multer';
import { PrismaService } from 'src/prisma/prisma.service';

@Injectable()
export class S3Service {
  private s3Client: S3Client;

  constructor(private readonly prisma: PrismaService) {
    const accessKeyId = process.env.S3_ACCESS_KEY;
    const secretAccessKey = process.env.S3_SECRET_KEY;

    if (!accessKeyId || !secretAccessKey) {
      throw new Error('S3 credentials are not defined in .env');
    }

    this.s3Client = new S3Client({
      endpoint: process.env.S3_ENDPOINT || 'http://localhost:9000',
      region: process.env.S3_REGION || 'us-east-1',
      credentials: {
        accessKeyId: accessKeyId,
        secretAccessKey: secretAccessKey,
      },
      forcePathStyle: true,
    });
  }

  async uploadFile(file: Express.Multer.File, userId: string) {
    const bucket = process.env.S3_BUCKET;

    if (!bucket) {
      throw new Error('S3_BUCKET is not defined');
    }

    const key = `${Date.now()}-${file.originalname}`;

    await this.s3Client.send(
      new PutObjectCommand({
        Bucket: bucket,
        Key: key,
        Body: file.buffer,
        ContentType: file.mimetype,
      }),
    );

    const command = new GetObjectCommand({
      Bucket: bucket,
      Key: key,
    });

    const signedUrl = await getSignedUrl(this.s3Client, command, {
      expiresIn: 99999,
    });

    await this.prisma.deaths.update({
      where: { id: userId },
      data: { imageUrl: key },
    });

    return {
      url: signedUrl,
      key: key,
    };
  }

  async getFileUrl(userId: string) {
    const bucket = process.env.S3_BUCKET;
    if (!bucket) throw new Error('S3_BUCKET is not defined');

    const deathRecord = await this.prisma.deaths.findUnique({
      where: { id: userId },
    });

    if (!deathRecord || !deathRecord.imageUrl) {
      throw new NotFoundException('Картинка для этого пользователя не найдена');
    }

    const command = new GetObjectCommand({
      Bucket: bucket,
      Key: deathRecord.imageUrl,
    });

    return await getSignedUrl(this.s3Client, command, { expiresIn: 99999 });
  }
}

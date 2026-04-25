import {
  Controller,
  Post,
  UploadedFile,
  UseInterceptors,
  BadRequestException,
  Param,
  Get,
  NotFoundException,
} from '@nestjs/common';
import { FileInterceptor } from '@nestjs/platform-express';
import {
  ApiConsumes,
  ApiBody,
  ApiTags,
  ApiOperation,
  ApiParam,
} from '@nestjs/swagger'; // Импортируем декораторы
import { S3Service } from './s3.service';

@ApiTags('Files') // Группировка в UI
@Controller('files')
export class S3Controller {
  constructor(private readonly s3Service: S3Service) {}

  @Post(':id/upload')
  @ApiOperation({ summary: 'Загрузка файла в S3' })
  @ApiConsumes('multipart/form-data') // Указываем тип контента для Swagger
  @ApiBody({
    schema: {
      type: 'object',
      properties: {
        file: {
          type: 'string',
          format: 'binary',
        },
      },
    },
  })
  @UseInterceptors(FileInterceptor('file'))
  async uploadFile(
    @Param('id') userId: string,
    @UploadedFile() file: Express.Multer.File,
  ) {
    if (!file) {
      throw new BadRequestException('Файл не найден');
    }
    return await this.s3Service.uploadFile(file, userId);
  }

  @Get('url/:id')
  @ApiOperation({ summary: 'Получить временную ссылку на файл из S3' })
  async getFileUrl(@Param('id') id: string) {
    const url = await this.s3Service.getFileUrl(id);
    return { url };
  }
}

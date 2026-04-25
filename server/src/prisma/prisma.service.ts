import { Injectable } from '@nestjs/common';
import { PrismaClient } from '@prisma/client';
import { PrismaBetterSqlite3 } from '@prisma/adapter-better-sqlite3';

@Injectable()
export class PrismaService extends PrismaClient {
  constructor() {
    if (!process.env.DATABASE_URL) {
      throw new Error('DATABASE_URL env variable is not set');
    }

    const adapter = new PrismaBetterSqlite3({
      url: process.env.DATABASE_URL,
    });

    super({ adapter });
  }
}

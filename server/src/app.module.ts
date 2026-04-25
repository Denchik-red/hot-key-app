import { Module } from '@nestjs/common';
import { PrismaModule } from './prisma/prisma.module';
import { ConfigModule } from '@nestjs/config';
import { DeathsModule } from './deaths/deaths.module';
import { ObsModule } from './obs/obs.module';
import { S3Module } from './s3/s3.module';

@Module({
  imports: [
    PrismaModule,
    ConfigModule.forRoot({
      isGlobal: true,
    }),
    DeathsModule,
    ObsModule,
    S3Module,
  ],
  controllers: [],
  providers: [],
})
export class AppModule {}

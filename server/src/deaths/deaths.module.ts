import { Module } from '@nestjs/common';
import { DeathsService } from './deaths.service';
import { DeathsController } from './deaths.controller';

@Module({
  controllers: [DeathsController],
  providers: [DeathsService],
})
export class DeathsModule {}

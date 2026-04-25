import { Module, Global } from '@nestjs/common';
import { ObsService } from './obs.service';
import { ObsController } from './obs.controller';

@Global()
@Module({
  controllers: [ObsController],
  providers: [ObsService],
  exports: [ObsService],
})
export class ObsModule {}

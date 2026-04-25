import {
  Controller,
  Get,
  Post,
  Body,
  Delete,
  Param,
  Patch,
} from '@nestjs/common';
import { DeathsService } from './deaths.service';
import { ApiOkResponse, ApiOperation, ApiTags } from '@nestjs/swagger';
import { DeathEntity } from './entities/death.entity';
import { CreateDeathDto } from './dto/create-death.dto';

@ApiTags('deaths')
@Controller('deaths')
export class DeathsController {
  constructor(private readonly deathsService: DeathsService) {}
  @ApiOperation({ summary: 'Получить список всех участников и их смерти' })
  @ApiOkResponse({
    type: DeathEntity,
    isArray: true,
    description: 'Список смертей успешно получен',
  })
  @Get()
  findAll(): Promise<DeathEntity[]> {
    return this.deathsService.findAll();
  }

  @ApiOperation({ summary: 'Получить участника' })
  @ApiOkResponse({
    type: DeathEntity,
  })
  @Get(':id')
  findUnique(@Param('id') id: string): Promise<DeathEntity> {
    return this.deathsService.findUnique(id);
  }

  @ApiOperation({ summary: 'Получить смерти участника' })
  @ApiOkResponse({
    type: Number,
    description: 'Количество смертей успешно получено',
  })
  @Get(':id/count')
  getDeathCount(@Param('id') id: string): Promise<number> {
    return this.deathsService.getDeathCount(id);
  }

  @ApiOperation({ summary: 'Создать нового участника' })
  @ApiOkResponse({
    type: DeathEntity,
    description: 'участник успешно создан',
  })
  @Post()
  create(@Body() createDeathDto: CreateDeathDto): Promise<DeathEntity> {
    return this.deathsService.create(createDeathDto);
  }

  @ApiOperation({
    summary: 'Удалить участника по id',
  })
  @ApiOkResponse({
    description: 'участник успешно удален',
  })
  @Delete(':id')
  async delete(@Param('id') id: string) {
    await this.deathsService.delete(id);
  }

  @ApiOperation({ summary: 'Увеличить количество смертей участника на 1' })
  @ApiOkResponse({
    type: DeathEntity,
    description: 'Смерть успешно добавлена',
  })
  @Patch('inc/:id')
  async incrementDeath(@Param('id') id: string): Promise<DeathEntity> {
    return this.deathsService.incrementDeath(id);
  }

  @ApiOperation({ summary: 'Уменьшить количество смертей участника на 1' })
  @ApiOkResponse({
    type: DeathEntity,
    description: 'Смерть успешно убрана',
  })
  @Patch('dec/:id')
  async decrementDeath(@Param('id') id: string): Promise<DeathEntity> {
    return this.deathsService.decrementDeath(id);
  }

  @ApiOperation({ summary: 'Очистить смерти участника' })
  @ApiOkResponse({
    description: 'Смерти успешно очищены',
  })
  @Delete(':id/clean')
  async cleanDeathCount(@Param('id') id: string): Promise<void> {
    await this.deathsService.cleanDeathCount(id);
  }
}

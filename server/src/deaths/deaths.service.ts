import { Injectable, NotFoundException } from '@nestjs/common';
import { PrismaService } from 'src/prisma/prisma.service';
import { DeathEntity } from './entities/death.entity';
import { CreateDeathDto } from './dto/create-death.dto';
import { ObsService } from 'src/obs/obs.service';

@Injectable()
export class DeathsService {
  constructor(
    private readonly prisma: PrismaService,
    private readonly obsService: ObsService,
  ) {}

  async findAll(): Promise<DeathEntity[]> {
    const deaths = await this.prisma.deaths.findMany();
    return deaths.map((death) => new DeathEntity(death));
  }

  async findUnique(userId: string): Promise<DeathEntity> {
    const death = await this.prisma.deaths.findUnique({
      where: { id: userId },
    });

    if (!death) throw new NotFoundException('Пользователь не найден');

    return new DeathEntity(death);
  }

  async create(createDeathDto: CreateDeathDto): Promise<DeathEntity> {
    const death = await this.prisma.deaths.create({
      data: createDeathDto,
    });
    return new DeathEntity(death);
  }

  async delete(userId: string): Promise<void> {
    await this.prisma.deaths.delete({
      where: { id: userId },
    });
  }

  async incrementDeath(userId: string): Promise<DeathEntity> {
    const updatedState = await this.prisma.deaths.update({
      where: { id: userId },
      data: { count: { increment: 1 } },
    });

    this.obsService
      .updateTestText(updatedState.count.toString(), updatedState.id)
      .catch((err) => {
        console.error('Не удалось обновить OBS:', err.message);
      });

    return new DeathEntity(updatedState);
  }

  async decrementDeath(userId: string): Promise<DeathEntity> {
    const updatedState = await this.prisma.deaths.update({
      where: { id: userId },
      data: { count: { decrement: 1 } },
    });

    this.obsService
      .updateTestText(updatedState.count.toString(), updatedState.id)
      .catch((err) => {
        console.error('Не удалось обновить OBS:', err.message);
      });
    return new DeathEntity(updatedState);
  }

  async getDeathCount(userId: string): Promise<number> {
    const death = await this.prisma.deaths.findUnique({
      where: { id: userId },
      select: { count: true, id: true },
    });

    if (!death) {
      throw new NotFoundException(`Участник с ID ${userId} не найден.`);
    }

    this.obsService
      .updateTestText(death.count.toString(), death.id)
      .catch((err) => {
        console.error('Не удалось обновить OBS:', err.message);
      });

    return death.count;
  }

  async cleanDeathCount(userId: string): Promise<void> {
    const data = await this.prisma.deaths.update({
      where: { id: userId },
      data: { count: 0 },
    });

    try {
      this.obsService.updateTestText('0', data.id);
    } catch (err) {
      console.log(err);
    }
  }
}

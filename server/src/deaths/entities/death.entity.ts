import { ApiProperty } from '@nestjs/swagger';
export class DeathEntity {
  @ApiProperty()
  id!: string;

  @ApiProperty()
  userName!: string;

  @ApiProperty()
  count!: number;

  @ApiProperty()
  imageUrl!: string | null;

  constructor(partial: Partial<DeathEntity>) {
    Object.assign(this, partial);
  }
}

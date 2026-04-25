import { ApiProperty } from '@nestjs/swagger';
import {
  IsNotEmpty,
  IsNumber,
  IsOptional,
  IsString,
  Min,
} from 'class-validator';

export class CreateDeathDto {
  @ApiProperty({
    description: 'Id текста в OBS',
    example: 'Test_Text',
  })
  @IsString()
  @IsNotEmpty()
  id!: string;

  @ApiProperty({
    description: 'Имя пользователя',
    example: 'Den_Lox',
  })
  @IsString()
  @IsNotEmpty()
  userName!: string;

  @ApiProperty({
    description: 'Количество смертей',
    example: 0,
  })
  @IsNumber()
  @IsOptional()
  @Min(0)
  count: number = 0;
}

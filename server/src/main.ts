import { NestFactory } from '@nestjs/core';
import { AppModule } from './app.module';
import { DocumentBuilder, SwaggerModule } from '@nestjs/swagger';

async function bootstrap() {
  const app = await NestFactory.create(AppModule);

  app.enableCors({
    origin: '*',
    methods: 'GET,HEAD,PUT,PATCH,POST,DELETE',
  });

  const swaggerConfig = new DocumentBuilder()
    .setTitle('OBS Death Counter API')
    .setDescription('API for managing death counts in OBS')
    .setVersion('1.0')
    .build();

  const documentFactory = SwaggerModule.createDocument(app, swaggerConfig);
  SwaggerModule.setup('/docs', app, documentFactory);

  await app.listen(process.env.PORT ?? 3000);
}
bootstrap();

import { Injectable, Logger } from '@nestjs/common';
import OBSWebSocket from 'obs-websocket-js';
import { text } from 'stream/consumers';

@Injectable()
export class ObsService {
  private obs = new OBSWebSocket();
  private readonly logger = new Logger(ObsService.name);

  private readonly OBS_ADDRESS = `${process.env.OBS_HOST}:${process.env.OBS_PORT}`;
  private readonly OBS_PASSWORD = process.env.OBS_PASSWORD;

  async onModuleInit() {
    try {
      await this.obs.connect(this.OBS_ADDRESS, this.OBS_PASSWORD);
      this.logger.log('✅ Успешно подключено к OBS WebSocket');
    } catch (error) {
      if (error instanceof Error) {
        this.logger.error('❌ Ошибка подключения к OBS:', error.message);
      } else {
        this.logger.error('❌ Произошла неизвестная ошибка', String(error));
      }
    }
  }

  async updateTestText(newText: string, textId: string) {
    try {
      await this.obs.call('SetInputSettings', {
        inputName: textId,
        inputSettings: {
          text: newText,
        },
      });

      this.logger.log(`Текст в источнике ${textId} изменен на: ${newText}`);
      return { success: true, newValue: newText };
    } catch (error) {
      this.logger.error(
        'Ошибка при обновлении текста:',
        error instanceof Error ? error.message : String(error),
      );
      throw error;
    }
  }
}

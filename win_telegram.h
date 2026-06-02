#ifndef WIN_TELEGRAM_H
#define WIN_TELEGRAM_H

int send_telegram_text(const char* message);
int send_telegram_file(const char* filePath);

#endif // WIN_TELEGRAM_H
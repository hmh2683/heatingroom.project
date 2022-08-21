#ifndef SRC_FND_CONTROLLER_H_
#define SRC_FND_CONTROLLER_H_

#include "main.h"
#include "defines.h"

void Fnd_Init(SPI_HandleTypeDef * hspi);
void send(uint8_t X);
void sendPort(uint8_t X, uint8_t port);
void digitTemp(int temp);

#endif

#ifndef PATCH_SERVICE_H_
#define PATCH_SERVICE_H_
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*
Use socket or Uart transfer patch
*/

typedef struct service_context
{
	int pos;
	int sockfd;
	int status;
} service_context;


typedef struct patch_service
{
	service_context *ctx;
	bool (*setup_connect)(service_context *ctx);
	size_t (*receive_buf)(service_context *ctx, uint8_t *buf, size_t len);
	size_t (*send_buf)(service_context *ctx, uint8_t *buf, size_t len);
	void (*dis_connect)(service_context *ctx);
} patch_service;

typedef struct __attribute__((__packed__)) patch_desc {
	uint16_t type;
	uint16_t code_len;
	union {
		struct {
			uint16_t opt: 4;
			uint16_t rtype: 4;
			uint16_t prioriy: 4;
			uint16_t iters: 16;
		};
		uint32_t settings;
	};
	uint32_t sign;
	union {
		uint32_t fixed_id;
		uint32_t inst_addr;
	};
	uint8_t code[0];
} patch_desc;

typedef struct __attribute__((aligned(2))) patch_payload {
	uint8_t sign[16];
	// uint16_t pkt_len;
	uint8_t *pkt;
} patch_payload;

// run in new task
bool start_patch_service(void);
bool query_service_alive(void);

#endif
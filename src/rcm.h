/*
 * Copyright (c) 2011, NVIDIA CORPORATION
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _RCM_H
#define _RCM_H

#include <stdint.h>

#define RCM_MIN_MSG_LENGTH 1024 // In bytes

#define NVBOOT_VERSION(a,b) ((((a)&0xffff) << 16) | ((b)&0xffff))
#define RCM_VERSION_1 (NVBOOT_VERSION(1, 0))
#define RCM_VERSION_35 (NVBOOT_VERSION(0x35, 1))
#define RCM_VERSION_MAJOR(ver) ((ver) >> 16)
#define RCM_VERSION_MINOR(ver) ((ver) & 0xffff)

// recovery mode commands
#define RCM_CMD_NONE               0x0
#define RCM_CMD_SYNC               0x1
#define RCM_CMD_DL_MINILOADER      0x4
#define RCM_CMD_QUERY_BR_VERSION   0x5
#define RCM_CMD_QUERY_RCM_VERSION  0x6
#define RCM_CMD_QUERY_BD_VERSION   0x7

// AES block size in bytes
#define RCM_AES_BLOCK_SIZE      (128 / 8)

/*
 * Defines the header for RCM messages from the host.
 * Messages from the host have the format:
 *     rcm_msg_t
 *     payload
 *     padding
 */
typedef struct {
	uint32_t len_insecure;
	uint8_t cmac_hash[RCM_AES_BLOCK_SIZE];
	uint8_t reserved[16];
	uint32_t opcode;
	uint32_t len_secure;
	uint32_t payload_len;
	uint32_t rcm_version;
	uint8_t args[48];
	uint8_t padding[16];
} rcm1_msg_t;

typedef struct {
	uint32_t len_insecure;
	uint8_t modulus[2048 / 8];
	union {
		uint8_t cmac_hash[RCM_AES_BLOCK_SIZE];
		uint8_t rsa_pss_sig[2048 / 8];
	} object_sig;
	uint8_t reserved[16];
	uint32_t ecid[4];
	uint32_t opcode;
	uint32_t len_secure;
	uint32_t payload_len;
	uint32_t rcm_version;
	uint8_t args[48];
	uint8_t padding[16];
} rcm35_msg_t;

// security operating modes
#define RCM_OP_MODE_PRE_PRODUCTION  0x1
#define RCM_OP_MODE_DEVEL           0x3
#define RCM_OP_MODE_ODM_OPEN        0x5

int rcm_init(uint32_t version);
uint32_t rcm_get_msg_len(uint8_t *msg);
int rcm_create_msg(
	uint32_t opcode,
	uint8_t *args,
	uint32_t args_len,
	uint8_t *payload,
	uint32_t payload_len,
	uint8_t **msg);

#endif // _RCM_H

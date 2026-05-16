#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
 * 14 紧凑网络协议头解析器（位域）
 * 要求：
 *  - 协议头格式：2 字节版本（4 位主版本 + 4 位次版本）、2 字节长度（16 位无符号）、1 字节标志（3 位保留 + 5
 * 位功能标志）。
 *  - 输入示例：\x00\x03\x00\x20\x00  →  version:0.3, length:32, flags:0x00
 *  - 考察点：位域定义、结构体对齐（#pragma pack(1)）、字节流解析
 */

/* 结构体对齐：按 1 字节对齐，确保与网络字节流逐字节一致 */
#pragma pack(push, 1)

/*
 * 原始协议头（与网络字节流逐字节对应，不直接使用位域跨字节）
 */
typedef struct {
    uint8_t version;
    uint8_t length_be[2];
    uint8_t flags_raw;
} proto_header_raw_t;

/*
 * 位域定义的逻辑视图（使用位域表达语义，不强行与字节流直接映射）
 * 注意：位域在不同平台的位序实现可能不同，故此处不作为内存映射，仅用于展示语义并由解析代码赋值。
 */
typedef struct {
    unsigned ver_major : 4;
    unsigned ver_minor : 4;
    uint16_t length;
    unsigned flags : 5;
} proto_header_bits_t;

#pragma pack(pop)

/*
 * 将网络序（大端）的 16 位数转换为主机序
 */
static uint16_t be16_to_cpu(uint16_t be) {
    return (be >> 8) | (be << 8);
}

int main(void) {
    const uint8_t stream[5] = {0x00, 0x03, 0x00, 0x20, 0x00};

    proto_header_raw_t raw = {0};
    memcpy(&raw, stream, sizeof(raw));

    unsigned ver_major = (raw.version >> 4) & 0x0F;
    unsigned ver_minor = raw.version & 0x0F;

    uint16_t length = (raw.length_be[0] << 8) | raw.length_be[1];
    unsigned flags = raw.flags_raw & 0x1F;

    proto_header_bits_t view = {
        .ver_major = ver_major,
        .ver_minor = ver_minor,
        .length = length,
        .flags = flags
    };

    printf("version:%u.%u, length:%u, flags:0x%02X\n", view.ver_major, view.ver_minor, view.length, view.flags & 0xFFu);

    return 0;
}

#include <stdio.h>

typedef unsigned char word8;
typedef unsigned int word32;
typedef __int8_t uint8_t;

#define Nb 4

/*
 * Nb是state的列数，Nb = 块长 / 32， 明文应该被划分成4*Nb的矩阵
 * Nk是加密密钥的列数，Nk = 密钥长 / 32
 * Nr是轮数， Nr =
 */
const uint8_t SBox[256] = {
    /*  0     1     2     3     4     5     6     7     8     9     a     b     c     d     e      f  */
    0x63, 0x7C, 0x5D, 0x42, 0x1F, 0x00, 0x21, 0x3E, 0x9B, 0x84, 0xA5, 0xBA, 0xE7, 0xF8, 0xD9, 0xC6,
    0x92, 0x8D, 0xAC, 0xB3, 0xEE, 0xF1, 0xD0, 0xCF, 0x6A, 0x75, 0x54, 0x4B, 0x16, 0x09, 0x28, 0x37,
    0x80, 0x9F, 0xBE, 0xA1, 0xFC, 0xE3, 0xC2, 0xDD, 0x78, 0x67, 0x46, 0x59, 0x04, 0x1B, 0x3A, 0x25,
    0x71, 0x6E, 0x4F, 0x50, 0x0D, 0x12, 0x33, 0x2C, 0x89, 0x96, 0xB7, 0xA8, 0xF5, 0xEA, 0xCB, 0xD4,
    0xA4, 0xBB, 0x9A, 0x85, 0xD8, 0xC7, 0xE6, 0xF9, 0x5C, 0x43, 0x62, 0x7D, 0x20, 0x3F, 0x1E, 0x01,
    0x55, 0x4A, 0x6B, 0x74, 0x29, 0x36, 0x17, 0x08, 0xAD, 0xB2, 0x93, 0x8C, 0xD1, 0xCE, 0xEF, 0xF0,
    0x47, 0x58, 0x79, 0x66, 0x3B, 0x24, 0x05, 0x1A, 0xBF, 0xA0, 0x81, 0x9E, 0xC3, 0xDC, 0xFD, 0xE2,
    0xB6, 0xA9, 0x88, 0x97, 0xCA, 0xD5, 0xF4, 0xEB, 0x4E, 0x51, 0x70, 0x6F, 0x32, 0x2D, 0x0C, 0x13,
    0xEC, 0xF3, 0xD2, 0xCD, 0x90, 0x8F, 0xAE, 0xBl, 0x14, 0x0B, 0x2A, 0x35, 0x68, 0x77, 0x56, 0x49,
    0x1D, 0x02, 0x23, 0x3C, 0x61, 0x7E, 0x5F, 0x40, 0xE5, 0xFA, 0xDB, 0xC4, 0x99, 0x86, 0xA7, 0xB8,
    0x0F, 0x10, 0x31, 0x2E, 0x73, 0x6C, 0x4D, 0x52, 0xF7, 0xE8, 0xC9, 0xD6, 0x8B, 0x94, 0xB5, 0xAA,
    0xFE, 0xE1, 0xC0, 0xDF, 0x82, 0x9D, 0xBC, 0xA3, 0x06, 0x19, 0x38, 0x27, 0x7A, 0x65, 0x44, 0x5B,
    0x2B, 0x34, 0x15, 0x0A, 0x57, 0x48, 0x69, 0x76, 0xD3, 0xCC, 0xED, 0xF2, 0xAF, 0xB0, 0x91, 0x8E,
    0xDA, 0xC5, 0xE4, 0xFB, 0xA6, 0xB9, 0x98, 0x87, 0x22, 0x3D, 0x1C, 0x03, 0x5E, 0x41, 0x60, 0x7F,
    0xC8, 0xD7, 0xF6, 0xE9, 0xB4, 0xAB, 0x8A, 0x95, 0x30, 0x2F, 0x0E, 0x11, 0x4C, 0x53, 0x72, 0x6D,
    0x39, 0x26, 0x07, 0x18, 0x45, 0x5A, 0x7B, 0x64, 0xC1, 0xDE, 0xFF, 0xE0, 0xBD, 0xA2, 0x83, 0x9C
};

const uint8_t invSBox[256] = {
    /*  0     1     2     3     4     5     6     7     8     9     a     b     c     d     e      f  */
    0x05, 0x4F, 0x91, 0xDB, 0x2C, 0x66, 0xB8, 0xF2, 0x57, 0x1D, 0xC3, 0x89, 0x7E, 0x34, 0xEA, 0xA0,
    0xA1, 0xEB, 0x35, 0x7F, 0x88, 0xC2, 0x1C, 0x56, 0xF3, 0xB9, 0x67, 0x2D, 0xDA, 0x90, 0x4E, 0x04,
    0x4C, 0x06, 0xD8, 0x92, 0x65, 0x2F, 0xF1, 0xBB, 0x1E, 0x54, 0x8A, 0xC0, 0x37, 0x7D, 0xA3, 0xE9,
    0xE8, 0xA2, 0x7C, 0x36, 0xC1, 0x8B, 0x55, 0x1F, 0xBA, 0xF0, 0x2E, 0x64, 0x93, 0xD9, 0x07, 0x4D,
    0x97, 0xDD, 0x03, 0x49, 0xBE, 0xF4, 0x2A, 0x60, 0xC5, 0x8F, 0x51, 0x1B, 0xEC, 0xA6, 0x78, 0x32,
    0x33, 0x79, 0xA7, 0xED, 0x1A, 0x50, 0x8E, 0xC4, 0x61, 0x2B, 0xF5, 0xBF, 0x48, 0x02, 0xDC, 0x96,
    0xDE, 0x94, 0x4A, 0x00, 0xF7, 0xBD, 0x63, 0x29, 0x8C, 0xC6, 0x18, 0x52, 0xA5, 0xEF, 0x31, 0x7B,
    0x7A, 0x30, 0xEE, 0xA4, 0x53, 0x19, 0xC7, 0x8D, 0x28, 0x62, 0xBC, 0xF6, 0x01, 0x4B, 0x95, 0xDF,
    0x20, 0x6A, 0xB4, 0xFE, 0x09, 0x43, 0x9D, 0xD7, 0x72, 0x38, 0xE6, 0xAC, 0x5B, 0x11, 0xCF, 0x85,
    0x84, 0xCE, 0x10, 0x5A, 0xAD, 0xE7, 0x39, 0x73, 0xD6, 0x9C, 0x42, 0x08, 0xFF, 0xB5, 0x6B, 0x21,
    0x69, 0x23, 0xFD, 0xB7, 0x40, 0x0A, 0xD4, 0x9E, 0x3B, 0x71, 0xAF, 0xE5, 0x12, 0x58, 0x86, 0xCC,
    0xCD, 0x87, 0x59, 0x13, 0xE4, 0xAE, 0x70, 0x3A, 0x9F, 0xD5, 0x0B, 0x41, 0xB6, 0xFC, 0x22, 0x68,
    0xB2, 0xF8, 0x26, 0x6C, 0x9B, 0xD1, 0x0F, 0x45, 0xE0, 0xAA, 0x74, 0x3E, 0xC9, 0x83, 0x5D, 0x17,
    0x16, 0x5C, 0x82, 0xC8, 0x3F, 0x75, 0xAB, 0xE1, 0x44, 0x0E, 0xD0, 0x9A, 0x6D, 0x27, 0xF9, 0xB3,
    0xFB, 0xB1, 0x6F, 0x25, 0xD2, 0x98, 0x46, 0x0C, 0xA9, 0xE3, 0x3D, 0x77, 0x80, 0xCA, 0x14, 0x5E,
    0x5F, 0x15, 0xCB, 0x81, 0x76, 0x3C, 0xE2, 0xA8, 0x0D, 0x47, 0x99, 0xD3, 0x24, 0x6E, 0xB0, 0xFA
};

/*
 * SRD是S盒，用于状态矩阵的转换
 * 输入输出是8bit的字节数组， 对于加密，输入是明文块和密钥   对于解密，输入是密文块和密钥输出是明文块
 * state是一个四行Nb列的矩阵
 */

void SubBytes(uint8_t *state) {               //字节分组
    int i, j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < Nb; ++j) {              //i行j列
            state[i * Nb + j] = SBox[state[i * Nb + j]];                //置换
        }
    }
}

void InvSubBytes(uint8_t *state) {
    int i, j;
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < Nb; ++j)
            state[i * Nb + j] = invSBox[state[i * Nb + j]];
    }
}

void ShiftRows(uint8_t *state) {
    /*
     * Nb   C0  C1  C2  C3
     * 4    0   1   2   3
     * 5    0   1   2   3
     * 6    0   1   2   3
     * 7    0   1   2   4
     * 8    0   1   3   4
     */
    //state[i][j] = state[(j+(Nb-offset))%Nb]
    int i, j;
    int offset = 0;
    uint8_t tmp[Nb];
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < Nb; ++j) tmp[j] = state[i * Nb + j];
        for (j = 0; j < Nb; ++j) {
            state[i * Nb + j] = tmp[(j + offset) % Nb];
        }
        offset++;
    }
}

void InvShiftRows(uint8_t *state) {
    int i, j;
    int offset = 0;
    uint8_t tmp[Nb];
    for (i = 0; i < 4; ++i) {
        for (j = 0; j < Nb; ++j) tmp [j] = state[i * Nb + j];
        for (j = 0; j < Nb; ++j) {
            state[i * Nb + j] = tmp[(j + Nb - offset) % Nb];
        }
        offset++;
    }
}

void AddRoundKey() {                        //轮密钥加

}

void Round(State, ExpandedKey[i]) {         //轮函数
    SubBytes(State);
    ShiftRows(State);
    MixColumns(State);
    AddRoundKey(State, ExpandedKey[i]);
}

void FinalRound() {                         //最后一轮
    SubBytes(State);
    ShiftRows(State);
    AddRoundKey(State, ExpandedKey[Nr]);
}

void KeyExpansion() {                       //密钥扩展

}



int Encrypt(word a[4][]) {
    uint8_t state[4 * Nb];                      //4行 Nb列

    KeyExpansion(State, CipherKey);
    AddRoundKey(CipherKey, ExpandedKey);
    for (int i = 1, i < Nr; i++) Round(state, ExpandedKey[i]);
    FinalRound(State, ExpandedKey[Nr]);
}

int main() {
    int keylen = 0;

    uint8_t state[]
    return 0;
}

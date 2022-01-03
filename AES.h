#include <stdio.h>
#ifndef AES_AES_H
#define AES_AES_H

typedef unsigned char word8;
typedef unsigned int word32;

#define MAXBC 8
#define MAXKC 8
#define MAXROUNDS 14

/*
 * 输入两个word8，返回在GF (256)中的乘积
 * 用于 MixColumns和InvMixColumns的矩阵乘法
 */
word8 mul (word8 a, word8 b);

/*
 * state[4][MAXBC]是state矩阵，rk[4][MAXBC]是轮密钥
 * MAXBC是NK，即每次轮密钥加都只针对矩阵的MAXBC列
 */
void AddRoundKey(word8 state[4][MAXBC], word8 rk[4][MAXBC]);

/*
 * Replace every byte of the input by the byte at that place in the non-linear S-box
 * 输入state[4][MAXBC]为状态矩阵， 使用box对state进行非线性替换
 */
void SubBytes(word8 state[4][MAXBC], const word8 box[256]);

/*
 * d 是什么
 * ShiftRows负责对状态矩阵进行行移位， 移位的长度由行数和Nb决定
 */
void ShiftRows(word8 state[4][MAXBC], word8 d);

/*
 * Mix the four bytes of every column in a linear way
 */
void MixColumns(word8 state[4][MAXBC]);

/*
 * opposite operation of MixColumns
 */
void InvMixColumns(word8 state[4][MAXBC]);

/*
 * ExpandedKey为扩展密钥，MAXROUNDS + 1  * 4 * MAXBC的矩阵
 * Expanded[i]则为ExpandedKey[i]
 */
int KeyExpansion(word8 key[4][MAXKC], word8 ExpandedKey[MAXROUNDS + 1][4][MAXBC]);

/*
 * 矩阵state是待加密的明文块， rk是扩展后的密钥
 */
int Encrypt(word8 state[4][MAXBC], word8 rk[MAXROUNDS + 1][4][MAXBC]);

int Decrypt(word8 state[4][MAXBC], word8 rk[MAXROUNDS + 1][4][MAXBC]);

#endif //AES_AES_H

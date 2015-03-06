#!/bin/env python
# -*- coding: utf-8 -*-

import json
import zlib
import struct
import sys
sys.path.append('./')
from lite_encryption import *


def AddMagic(input):
    return '1Q2W3E4R'+input
def TestMagic(input):
    if '1Q2W3E4R'!=input[0:8]:
        return None
    else:
        return input[8:]

    
def AESEncrypt(input,key):
    cryto =AESEnc(key)
    length =struct.pack('!i',len(input))  # Input strings must be a multiple of 16 in length
    t =len(input)%16
    if t!=0:
        input =input+'_'*(16-t)
    ciphertext = cryto.Encrypt(input)
    return length+ciphertext
    
def AESDecrypt(input,key):
    cryto =AESDec(key)
    length =struct.unpack('!i',input[0:4])[0]
    plain = cryto.Decrypt(input[4:])
    return plain[0:length]
    
    
def TDESEncrypt(input,key):
    cryto =TDESEnc(key)
    length =struct.pack('!i',len(input))  # Input strings must be a multiple of 16 in length
    t =len(input)%16
    if t!=0:
        input =input+'_'*(16-t)
    ciphertext = cryto.Encrypt(input)
    return length+ciphertext
    
def TDESDecrypt(input,key):
    cryto =TDESDec(key)
    length =struct.unpack('!i',input[0:4])[0]
    plain = cryto.Decrypt(input[4:])
    return plain[0:length]
    

def AddChecksum(input):
    sum =zlib.adler32(input)
    return struct.pack('!i',sum)+input
    
def TestChecksum(input):
    sumPackage =struct.unpack('!i',input[0:4])[0]
    sumCal =zlib.adler32(input[4:])
    if sumCal!=sumPackage:
        print 'TestChecksum:  sumPackage=%s != sumCal=%s'%(sumPackage,sumCal)
        return None
    else:
        return input[4:]

def CompressData(input):
    length =struct.pack('!i',len(input))
    data =zlib.compress(input,9)
    return length+data
def UncompressData(input):
    length =struct.unpack('!i',input[0:4])[0]
    data = zlib.decompress(input[4:])
    return data
    
def Pack(data,key):
    data =AddMagic(data)
    data =CompressData(data)
    data =AddChecksum(data)
    data =TDESEncrypt(data,key)
    data =AESEncrypt(data,key)
    return data
def Unpack(data,key):
    data =AESDecrypt(data,key)
    if not data:
        return None
    data =TDESDecrypt(data,key)
    data =TestChecksum(data)
    if not data:
        return None
    data =UncompressData(data)
    data =TestMagic(data)
    if not data:
        return None
    return data



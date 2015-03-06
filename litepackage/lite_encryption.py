#!/bin/env python
# -*- coding: utf-8 -*-

from Crypto import Random
def RandomStr(len):
    return Random.new().read(len)
        
import base64
def Base64Encode(input):
    return base64.b64encode(input)
def Base64Decode(input):
    return base64.b64decode(input)

import binascii
def StrToHex(input):
    return binascii.unhexlify(input)
def HexToStr(input):
    return binascii.hexlify(input)
    
#import rsa
class RsaEnc:
    def __init__(self):
        self._pubkey = None
        self._prikey = None
    def InitPub(self,pubfile):
        pubdata = pubfile.read()
        pubfile.close()
        self._pubkey = rsa.PublicKey.load_pkcs1(pubdata)
    def InitPri(self,prifile):
        pridata = prifile.read()
        prifile.close()
        self._prikey = rsa.PrivateKey.load_pkcs1(pridata)
    def Encrypt(self,input):
        return rsa.encrypt(input, self._pubkey)
    def Decrypt(self,input):
        return rsa.decrypt(input, self._prikey)

from Crypto.Cipher import AES
class AESEnc:
    def __init__(self,key):
        self._key = key.ljust(32,'*')
        if len(self._key)>32:
            self._key =self._key[0:32]
    def Encrypt(self,input):
        iv = Random.new().read(AES.block_size)
        return iv+AES.new(self._key, AES.MODE_CBC, iv).encrypt(input)

class AESDec:
    def __init__(self,key):
        self._key = key.ljust(32,'*')
        if len(self._key)>32:
            self._key =self._key[0:32]
    def Decrypt(self,input):    
        iv = input[0:AES.block_size]
        return AES.new(self._key, AES.MODE_CBC, iv).decrypt(input[AES.block_size:])
        
        
from Crypto.Cipher import DES3
class TDESEnc:
    def __init__(self,key):
        self._key = key.ljust(DES3.block_size,'*')
        if len(self._key)>16:
            self._key =self._key[0:16]
    def Encrypt(self,input):
        iv = Random.new().read(DES3.block_size)
        return iv+DES3.new(self._key, DES3.MODE_CBC, iv).encrypt(input)
        
class TDESDec:
    def __init__(self,key):
        self._key = key.ljust(DES3.block_size,'*')
        if len(self._key)>16:
            self._key =self._key[0:16]
    def Decrypt(self,input):
        iv = input[0:DES3.block_size]
        return DES3.new(self._key, DES3.MODE_CBC, iv).decrypt(input[DES3.block_size:])
        
        
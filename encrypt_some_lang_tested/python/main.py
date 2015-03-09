import base64
from Crypto.Cipher import AES

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
    
input ='abcd'*4
key ='a'*16
iv ='x'*16
cipher =AES.new(key, AES.MODE_CBC, iv).encrypt(input)
print HexToStr(cipher)
print [ord(c) for c in cipher]
print AES.new(key, AES.MODE_CBC, iv).decrypt(cipher)

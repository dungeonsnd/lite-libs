
import lite_package
	
key =lite_package.RandomStr(32)
print 'key len=%d, hex encoded: %s\n'%(len(key),lite_package.HexToStr(key))

input='hello,world!'
print 'input len=%d: %s\n'%(len(input),input)

cipher =lite_package.Pack(input,key)
print 'cipher len=%d, base64 encoded: %s\n'%(len(cipher),lite_package.Base64Encode(cipher))
print 'cipher len=%d, hex encoded: %s\n'%(len(cipher),lite_package.HexToStr(cipher))

plain =lite_package.Unpack(cipher,key)
print 'plain len=%d: %s\n'%(len(plain),plain)


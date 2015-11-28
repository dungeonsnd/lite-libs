package main
  
import (
    "crypto/aes"
    "crypto/cipher"
    "fmt"
    "strings"
    "encoding/base64"
)

const (  
    base64Table = "123QRSTUabcdVWXYZHijKLAWDCABDstEFGuvwxyzGHIJklmnopqr234560178912"  
)  
  
var coder = base64.NewEncoding(base64Table)  
  
func base64Encode(src []byte) []byte {  
    return []byte(coder.EncodeToString(src))  
}  
  
func base64Decode(src []byte) ([]byte, error) {  
    return coder.DecodeString(string(src))  
}  

func main() {
    key := []byte{97,97,97,97,97,97,97,97,97,97,97,97,97,97,97,97}
    iv := []byte{120,120,120,120,120,120,120,120,120,120,120,120,120,120,120,120}
    fmt.Println("aes.BlockSize=", aes.BlockSize)

    cleartext := make([]byte, aes.BlockSize)
    strings.NewReader("abcdabcdabcdabcd").Read(cleartext)

    ciphertext := make([]byte, aes.BlockSize)

    block, err := aes.NewCipher(key)
    if err != nil {
        return 
    }
    
    aes := cipher.NewCBCEncrypter(block, iv)
    aes.CryptBlocks(ciphertext, cleartext)

    fmt.Println("明文：", cleartext)
    fmt.Println("密文 Base64：", string(base64Encode(ciphertext)))
    fmt.Println("密文：", ciphertext)

    aes = cipher.NewCBCDecrypter(block, iv)
    aes.CryptBlocks(ciphertext, cleartext)
    
    fmt.Println("明文：", cleartext)

}



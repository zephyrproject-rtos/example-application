#define     BOOLEAN     0x01

#define     UINT2       0x02
#define     UINT4       0x03
#define     UINT8       0x04
#define     UINT12      0x05
#define     UINT16      0x06
#define     UINT24      0x07
#define     UINT32      0x08
#define     UINT48      0x09
#define     UINT64      0x0A
#define     UINT128     0x0B

#define     SINT8       0x0C
#define     SINT12      0x0D
#define     SINT16      0x0E
#define     SINT24      0x0F
#define     SINT32      0x10
#define     SINT48      0x11
#define     SINT64      0x12
#define     SNT128      0x13

#define     FLOAT32     0x14
#define     FLOAT64     0x15

#define     MEDFLOAT32  0x16
#define     MEDFLOAT64  0x17

#define     UINT16_2    0x18
#define     UTF8S       0x19
#define     UTF16S      0x1A
#define     STRUCT      0x1B
#define     MEDASN1     0x1C


/*TODO

    Créer un type/struct incluant le type (déclaré plus haut), ainsi que 
    sa taille pour le sizeof (example : SINT16 = 4 octets donc 4)
    
*/

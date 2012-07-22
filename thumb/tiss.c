#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXDATA 0x1000
#define MAXOUT 16

unsigned short data[MAXDATA];
unsigned char hit[MAXDATA];
unsigned char okay[MAXDATA];
unsigned char branch[MAXDATA];
unsigned short output[MAXDATA][MAXOUT];


unsigned int newhit;

unsigned int addr;
unsigned int pass;
unsigned int doff;
unsigned int dissadd;


//(define-table p [
char p_name[0x20][8]=
{
"mov",
"cmn",
"add",
"bic",
"mul",
"eor",
"sub",
"and",
"mvn",
"ror",
"cmp",
"rsb",
"btst",
"or",
"extu",
"max",
"bset",
"min",
"bclr",
"adds2",
"bchg",
"adds4",
"adds8",
"adds16",
"exts",
"neg",
"lsr",
"clz",
"lsl",
"brev",
"asr",
"abs"
};


#define P_MOV 0x00
#define P_CMN 0x01
#define P_ADD 0x02
#define P_BIC 0x03

#define P_MUL 0x04
#define P_EOR 0x05
#define P_SUB 0x06
#define P_AND 0x07

#define P_MVN 0x08
#define P_ROR 0x09
#define P_CMP 0x0A
#define P_RSB 0x0B

#define P_BTST 0x0C
#define P_OR 0x0D
#define P_EXTU 0x0E
#define P_MAX 0x0F

#define P_BSET 0x10
#define P_MIN 0x11
#define P_BCLR 0x12
#define P_ADDS2 0x13

#define P_BCHG 0x14
#define P_ADDS4 0x15
#define P_ADDS8 0x16
#define P_ADDS16 0x17

#define P_EXTS 0x18
#define P_NEG 0x19
#define P_LSR 0x1A
#define P_CLZ 0x1B

#define P_LSL 0x1C
#define P_BREV 0x1D
#define P_ASR 0x1E
#define P_ABS 0x1F

#define P_SIZE 0x20

//(define-table q [
//"mov", "add", "mul", "sub",
//"mvn", "cmp", "btst", "extu",
//"bset", "bclr", "bchg", "adds8",
//"exts", "lsr", "lsl", "asr"])
#define Q_MOV 0x0
#define Q_ADD 0x1
#define Q_MUL 0x2
#define Q_SUB 0x3

#define Q_MVN 0x4
#define Q_CMP 0x5
#define Q_BTST 0x6
#define Q_EXTU 0x7

#define Q_BSET 0x8
#define Q_BCLR 0x9
#define Q_BCHG 0xA
#define Q_ADDS8 0xB

#define Q_EXTS 0xC
#define Q_LSR 0xD
#define Q_LSL 0xE
#define Q_ASR 0xF
#define Q_SIZE 0x10


char cc_name[0x10][4]=
{
"eq",
"ne",
"lo",
"hs",
"mi",
"pl",
"vs",
"vc",
"hi",
"ls",
"ge",
"lt",
"gt",
"le",
"",
"xx"
};

#define CC_EQ 0x0
#define CC_NE 0x1
#define CC_LO 0x2
#define CC_HS 0x3
#define CC_MI 0x4
#define CC_PL 0x5
#define CC_VS 0x6
#define CC_VC 0x7
#define CC_HI 0x8
#define CC_LS 0x9
#define CC_GE 0xA
#define CC_LT 0xB
#define CC_GT 0xC
#define CC_LE 0xD
#define CC_UN 0xE
#define CC_XX 0xF


//-------------------------------------------------------------------
unsigned int adreg ( unsigned int rx )
{
    //not really used yet, when this gets hit you need to comment
    //the changing printf...printf("//changing  after validating
    //it is doing what it should
    switch(rx)
    {
        case 14:
        {
            printf("changing r%u",rx);
            rx=26;
            printf(", to r%u\n",rx);
            break;
        }
        case 26:
        {
            printf("changing r%u",rx);
            rx=14;
            printf(", to r%u\n",rx);
            break;
        }
        case 29:
        {
            printf("changing r%u",rx);
            rx=13;
            printf(", to r%u\n",rx);
            break;
        }
        default:
        {
            if(rx>=16)
            {
                printf("changing r%u",rx);
                rx=12;
                printf(", to r%u\n",rx);
            }
            break;
        }
    }
    return(rx);
}
//-------------------------------------------------------------------
unsigned int addr2doff ( unsigned int addr )
{
    return(addr>>1);
}
//-------------------------------------------------------------------
unsigned int doff2addr ( unsigned int doff )
{
    return(doff<<1);
}
//-------------------------------------------------------------------
unsigned int p2q ( unsigned int p )
{
    switch(p)
    {
        case P_MOV  : return(Q_MOV  );
        case P_ADD  : return(Q_ADD  );
        case P_MUL  : return(Q_MUL  );
        case P_SUB  : return(Q_SUB  );
        case P_MVN  : return(Q_MVN  );
        case P_CMP  : return(Q_CMP  );
        case P_BTST : return(Q_BTST );
        case P_EXTU : return(Q_EXTU );
        case P_BSET : return(Q_BSET );
        case P_BCLR : return(Q_BCLR );
        case P_BCHG : return(Q_BCHG );
        case P_ADDS8: return(Q_ADDS8);
        case P_EXTS : return(Q_EXTS );
        case P_LSR  : return(Q_LSR  );
        case P_LSL  : return(Q_LSL  );
        case P_ASR  : return(Q_ASR  );
        //default: return(Q_SIZE);
    }
    return(Q_SIZE);
}
//-------------------------------------------------------------------
#include "diss.c"
//-------------------------------------------------------------------
void show_output ( void )
{
    unsigned int ra;

    if(pass)
    {
        printf("// %u",output[doff][0]);
        for(ra=0;ra<output[doff][0];ra++)
        {
            printf(" 0x%04X",output[doff][ra+1]);
        }
        printf("\n");
        if(okay[doff])
        {
            printf("//no emit\n");
        }
        else
        {
            if(output[doff][0]==0)
            {
                printf("[0x%08X] no output\n",addr);
            }
            else
            {
                diss();
                for(ra=0;ra<output[doff][0];ra++)
                {
                    printf("emit1(0x%04X); ",output[doff][ra+1]);
                }
                printf("\n");
            }
        }
    }
}
//-------------------------------------------------------------------
unsigned int build_p5d16u ( unsigned int p, unsigned int rd, unsigned int u, unsigned short *out )
{
    //1110 10pp pppd dddd  uuuu uuuu uuuu uuuu  uuuu uuuu uuuu uuuu    "; %s{p} r%i{d}, #0x%08x{u}"
    p&=0x1F;
    rd&=0x1F;
    u&=0xFFFFFFFF;
    out[0]=0xE800|(p<<5)|rd;
    out[1]=u&0xFFFF;
    out[2]=(u>>16)&0xFFFF;
    return(3);
}
//-------------------------------------------------------------------
unsigned int build_pc5dai ( unsigned int p, unsigned int cc, unsigned int rd, unsigned int ra, unsigned int i, unsigned short *out )
{
    //1100 00pp pppd dddd  aaaa accc c1ii iiii      "; %s{p}%s{c} r%i{d}, r%i{a}, #%i{i}"
    p&=0x1F;
    cc&=0xF;
    rd&=0x1F;
    ra&=0x1F;
    i&=0x3F;
    out[0]=0xC000|(p<<5)|rd;
    out[1]=0x0040|(ra<<11)|(cc<<7)|i;
    return(2);
}
//-------------------------------------------------------------------
unsigned int build_pc5dab ( unsigned int p, unsigned int cc, unsigned int rd, unsigned int ra, unsigned int rb, unsigned short *out )
{
    //1100 00pp pppd dddd   aaaa accc c00b bbbb                           "; %s{p}%s{c} r%i{d}, r%i{a}, r%i{b}"
    p&=0x1F;
    cc&=0xF;
    rd&=0x1F;
    ra&=0x1F;
    rb&=0x1F;
    out[0]=0xC000|(p<<5)|rd;
    out[1]=0x0000|(ra<<11)|(cc<<7)|rb;
    return(2);
}
//-------------------------------------------------------------------
void pc3 ( unsigned int p, unsigned int cc, unsigned int rd, unsigned int ra, unsigned int rb )
{
    p&=0x1F;
    cc&=0xF;
    rd&=0x1F;
    ra&=0x1F;
    rb&=0x1F;

    rd=adreg(rd);
    ra=adreg(ra);
    rb=adreg(rb);

    if((rd==ra)&&(cc==CC_UN))
    {
        //010p pppp ssss dddd                                               "; %s{p} r%i{d}, r%i{s}"
        output[doff][0]=1;
        output[doff][1]=0x4000|(p<<8)|(rb<<4)|rd;
    }
    else
    {
        //1100 00pp pppd dddd   aaaa accc c00b bbbb                           "; %s{p}%s{c} r%i{d}, r%i{a}, r%i{b}"
        output[doff][0]=build_pc5dab(p,cc,rd,ra,rb,&output[doff][1]); //uses 2
        //output[doff][0]=2;
        //output[doff][1]=0xC000|(p<<5)|rd;
        //output[doff][2]=0x0000|(ra<<11)|(cc<<7)|rb;
    }
    show_output();
}
//-------------------------------------------------------------------
void pc2u ( unsigned int p, unsigned int cc, unsigned int rd, unsigned int ra, unsigned int u )
{
    unsigned int x;
    unsigned int q;

    p&=0x1F;
    cc&=0xF;
    rd&=0x1F;
    ra&=0x1F;

    rd=adreg(rd);
    ra=adreg(ra);

    q=p2q(p);
    if(((u&(~0x1F))==0)&&(rd<16)&&(cc==CC_UN)&&(q<Q_SIZE))
    {
        //011q qqqu uuuu dddd                                               "; %s{q} r%i{d}, #%i{u}"
        output[doff][0]=1;
        output[doff][1]=0x6000|(q<<9)|(u<<4)|(rd);
    }
    else
    if(((u&(~0x1F))==0)||((u&(~0x1F))==((~0)&(~0x1F))))
    {
        //1100 00pp pppd dddd  aaaa accc c1ii iiii                           "; %s{p}%s{c} r%i{d}, r%i{a}, #%i{i}"
        output[doff][0]=2;
        output[doff][1]=0xC000|(p<<5)|rd;
        output[doff][2]=0x0040|(ra<<11)|(cc<<7)|(u&0x3F);
    }
    else
    if(((u&0x7F)==u)&&(rd==ra))
    {
        //1011 00pp pppd dddd  iiii iiii iiii iiii                           "; %s{p} r%i{d}, #0x%04x{i}"
        output[doff][0]=2;
        output[doff][1]=0xB000|(p<<5)|rd;
        output[doff][2]=u;
    }
    else
    {
        printf("untested\n"); //remove once used
        x=0;
        output[doff][0]=x;
        //desired rd = ra op u
        //mov(rd,ra)
        //010p pppp ssss dddd                                               "; %s{p} r%i{d}, r%i{s}"
        if((rd<16)&&(ra<16))
        {
            printf("untested\n"); //remove once used
            x++; output[doff][x]=0x4000|(P_MOV<<8)|(ra<<4)|rd;
        }
        else
        {
            printf("untested\n"); //remove once used
            x+=build_pc5dab(P_MOV,CC_UN,rd,ra,ra,&output[doff][x+1]);
        }
        //mov(ra,u);
        //at this point because it needs to be a 32 bit, no shortcut
        x+=build_p5d16u(P_MOV,ra,u,&output[doff][x+1]);
        //opcc(rd,ra);
        x+=build_pc5dab(p,cc,rd,ra,ra,&output[doff][x+1]);
        //limit check x?  It should be okay
        output[doff][0]=x;
    }
    show_output();
}
//-------------------------------------------------------------------
void pop ( unsigned int rd )
{
    unsigned int x;
//0000 0010 1bb0 0000                                               "; push r%d{b*8}"
//0000 0010 0bb0 0000                                               "; pop  r%d{b*8}"
//0000 0011 1bb0 0000                                               "; pushl r%d{b*8}"
//0000 0011 0bb0 0000                                               "; popl  r%d{b*8}"
          //0000 r0
          //0010 r6
          //0100 r16
          //0110 r24

    //this is arm/thumb specific 16 registers, r16 on the gpu is trashed
    //so wont work for other architectures.
    if(rd>15)
    {
        //if(rd!=26)
        {
            fprintf(stderr,"There is no r%u to pop\n",rd);
            printf("There is no r%u to pop\n",rd);
        }
    }

    switch(rd)
    {
        case 0:
        {
            x=output[doff][0];
            output[doff][0]+=1;
            if(output[doff][0]>=MAXOUT)
            {
                fprintf(stderr,"0x%06X : maxout too big %u\n",addr,output[doff][0]);
                exit(1);
            }
            output[doff][x+1]=0x0200;
            break;
        }
        case 6:
        {
            x=output[doff][0];
            output[doff][0]+=1;
            if(output[doff][0]>=MAXOUT)
            {
                fprintf(stderr,"0x%06X : maxout too big %u\n",addr,output[doff][0]);
                exit(1);
            }
            output[doff][x+1]=0x0220;
            break;
        }
        default:
        {
            x=output[doff][0];
            output[doff][0]+=3;
            if(output[doff][0]>=MAXOUT)
            {
                fprintf(stderr,"0x%06X : maxout too big %u\n",addr,output[doff][0]);
                exit(1);
            }
            //pop(r16)
            output[doff][x+1]=0x0240;
            //mov(rd,r16);
            //1100 00pp pppd dddd   aaaa accc c00b bbbb    "; %s{p}%s{c} r%i{d}, r%i{a}, r%i{b}"
            build_pc5dab(P_MOV,CC_UN,rd,16,16,&output[doff][x+2]); //takes 2
            //output[doff][x+2]=0xC000|(P_MOV<<5)|rd;
            //output[doff][x+3]=0x0000|(16<<11)|(CC_UN<<7)|(16);
        }
    }
}
//-------------------------------------------------------------------
void push ( unsigned int rs )
{
    unsigned int x;
//0000 0010 1bb0 0000                                               "; push r%d{b*8}"
//0000 0010 0bb0 0000                                               "; pop  r%d{b*8}"
//0000 0011 1bb0 0000                                               "; pushl r%d{b*8}"
//0000 0011 0bb0 0000                                               "; popl  r%d{b*8}"
          //1000 r0
          //1010 r6
          //1100 r16
          //1110 r24

    //this is arm/thumb specific 16 registers, r16 on the gpu is trashed
    //so wont work for other architectures.
    if(rs>15)
    {
        if(rs!=26)
        {
            fprintf(stderr,"There is no r%u to push\n",rs);
            printf("There is no r%u to push\n",rs);
        }
    }

    x=output[doff][0];
    output[doff][0]+=3;
    if(output[doff][0]>=MAXOUT)
    {
        fprintf(stderr,"0x%06X : maxout too big %u\n",addr,output[doff][0]);
        exit(1);
    }
    //mov(r16,rs);
    //1100 00pp pppd dddd   aaaa accc c00b bbbb    "; %s{p}%s{c} r%i{d}, r%i{a}, r%i{b}"
    build_pc5dab(P_MOV,CC_UN,16,rs,rs,&output[doff][x+1]); //takes 2
    //output[doff][x+1]=0xC000|(P_MOV<<5)|16;
    //output[doff][x+2]=0x0000|(rs<<11)|(CC_UN<<7)|(rs);
    //push(r16)
    output[doff][x+3]=0x02C0;
}
//-------------------------------------------------------------------
void tst ( unsigned int ra, unsigned int rb )
{
    //1100 00pp pppd dddd   aaaa accc c00b bbbb    "; %s{p}%s{c} r%i{d}, r%i{a}, r%i{b}"
    //1100 00pp pppd dddd   aaaa accc c1ii iiii    "; %s{p}%s{c} r%i{d}, r%i{a}, #%i{i}"
    output[doff][0]=4;
    build_pc5dab(P_MOV,CC_UN,22,ra,rb,&output[doff][1]); //takes 2
    //output[doff][1]=0xC000|(P_AND<<5)|22;
    //output[doff][2]=0x0000|(ra<<11)|(CC_UN<<7)|rb;
    build_pc5dai(P_CMP,CC_UN,22,22,0,&output[doff][3]); //takes 2
    //output[doff][3]=0xC000|(P_CMP<<5)|22;
    //output[doff][4]=0x0040|(22<<11)|(CC_UN<<7)|0;
    show_output();
}
//-------------------------------------------------------------------
void movi ( unsigned int rd, unsigned int data )
{
    rd=adreg(rd);
    //1110 10pp pppd dddd  uuuu uuuu uuuu uuuu  uuuu uuuu uuuu uuuu
    output[doff][0]=build_p5d16u(P_MOV,rd,data,&output[doff][1]); //takes 3
    //output[doff][1]=0xE800|(0x00<<5)|reg;
    //output[doff][2]=(data&0xFFFF);
    //output[doff][3]=(data>>16)&0xFFFF;
    show_output();
}
//-------------------------------------------------------------------
void br ( unsigned int rs )
{
    //GOOD LUCK!

    rs=adreg(rs);
    //0000 0000 010d dddd                                               "; b r%i{d}"
    output[doff][0]=1;
    output[doff][1]=0x0040|rs;
    show_output();
}
//-------------------------------------------------------------------
void b ( unsigned int address )
{
    unsigned int ra,rb,rc;
    //1001 cccc 0ooo oooo   oooo oooo oooo oooo    b<cc> $+o*2
    output[doff][0]=2;
    output[doff][1]=0x9E00;
    output[doff][2]=0x0000;
    if(pass)
    {
        rc=addr2doff(address);
        if(rc==doff)
        {
        }
        else
        if(rc<doff)
        {
            rb=0;
            for(ra=rc;ra<doff;ra++)
            {
                if((output[ra][0]==0)&&(okay[ra]==0)&&(hit[ra]))
                {
                    printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",doff2addr(ra),addr);
                    rb=0;
                    break;
                }
                rb+=output[ra][0];
            }
            if(rb)
            {
                printf("//b backward %u 0x%X\n",rb,rb);
                rb=-rb;
                output[doff][1]|=(rb>>16)&0x7F;
                output[doff][2]|=rb&0xFFFF;
            }
        }
        else
        {
            rb=0;
            for(ra=doff;ra<rc;ra++)
            {
                if((output[ra][0]==0)&&(okay[ra]==0)&&(hit[ra]))
                {
                    printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",doff2addr(ra),addr);
                    rb=0;
                    break;
                }
                rb+=output[ra][0];
            }
            if(rb)
            {
                printf("//b forward %u 0x%X\n",rb,rb);
                output[doff][1]|=(rb>>16)&0x7F;
                output[doff][2]|=rb&0xFFFF;
            }
        }
    }
    show_output();
}
//-------------------------------------------------------------------
void bxx ( unsigned int cc, unsigned int address )
{
    unsigned int ra,rb,rc;
    //1001 cccc 0ooo oooo   oooo oooo oooo oooo    b<cc> $+o*2
    output[doff][0]=2;
    output[doff][1]=0x9000|(cc<<8);
    output[doff][2]=0x0000;
    if(pass)
    {
        rc=addr2doff(address);
        if(rc==doff)
        {
        }
        else
        if(rc<doff)
        {
            rb=0;
            for(ra=rc;ra<doff;ra++)
            {
                if((output[ra][0]==0)&&(okay[ra]==0)&&(hit[ra]))
                {
                    printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",doff2addr(ra),addr);
                    rb=0;
                    break;
                }
                rb+=output[ra][0];
            }
            if(rb)
            {
                printf("//bxx backward %u 0x%X\n",rb,rb);
                rb=-rb;
                output[doff][1]|=(rb>>16)&0x7F;
                output[doff][2]|=rb&0xFFFF;
            }
        }
        else
        {
            rb=0;
            for(ra=doff;ra<rc;ra++)
            {
                if((output[ra][0]==0)&&(okay[ra]==0)&&(hit[ra]))
                {
                    printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",doff2addr(ra),addr);
                    rb=0;
                    break;
                }
                rb+=output[ra][0];
            }
            if(rb)
            {
                printf("//bxx forward %u 0x%X\n",rb,rb);
                output[doff][1]|=(rb>>16)&0x7F;
                output[doff][2]|=rb&0xFFFF;
            }
        }
    }
    show_output();
}
//-------------------------------------------------------------------
void bl ( unsigned int address )
{
    unsigned int ra,rb,rc;

    //1001 oooo 1ooo oooo   oooo oooo oooo oooo
    output[doff][0]=2;
    output[doff][1]=0x9080;
    output[doff][2]=0x0000;
    if(pass)
    {
        rc=addr2doff(address);
        if(rc==doff)
        {
            printf("// bl to self\n");
        }
        else
        if(rc<doff)
        {
            rb=0;
            for(ra=rc;ra<doff;ra++)
            {
                if((output[ra][0]==0)&&(okay[ra]==0)&&(hit[ra]))
                {
                    printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",doff2addr(ra),addr);
                    rb=0;
                    break;
                }
                rb+=output[ra][0];
            }
            if(rb)
            {
                printf("//bl backward %u 0x%X\n",rb,rb);
                rb=-rb;
                output[doff][1]|=(rb>>16)&0x0F7F;
                output[doff][2]|=rb&0xFFFF;
            }
            else
            {
                printf("rb=0;\n");
            }
        }
        else
        {
            rb=0;
            for(ra=doff;ra<rc;ra++)
            {
                if((output[ra][0]==0)&&(okay[ra]==0)&&(hit[ra]))
                {
                    printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",doff2addr(ra),addr);
                    rb=0;
                    break;
                }
                rb+=output[ra][0];
            }
            if(rb)
            {
                printf("//bl forward %u 0x%X\n",rb,rb);
                output[doff][1]|=(rb>>16)&0x0F7F;
                output[doff][2]|=rb&0xFFFF;
            }
            else
            {
                printf("rb=0;\n");
            }
        }
    }
    show_output();
}
//-------------------------------------------------------------------
void rts ( void )
{
//0000 0000 0101 1010                                               "; rts"     #otherwise known as 'b lr' (b r26)
//0000 0000 010d dddd                                               "; b r%i{d}"
    output[doff][0]=1;
    output[doff][1]=0x005A;
    show_output();
}
//-------------------------------------------------------------------
void ldr ( unsigned int dat_reg, unsigned int add_reg, unsigned int offset )
{
//0000 1000 ssss dddd                                               "; ld%s{w} r%i{d}, (r%i{s})"
//0000 1001 ssss dddd                                               "; st%s{w} r%i{d}, (r%i{s})"
//0010 uuuu ssss dddd                                               "; ld  r%i{d}, 0x%02x{u*4}(r%i{s})"
//0011 uuuu ssss dddd                                               "; st  r%i{d}, 0x%02x{u*4}(r%i{s})"
//1010 0010 ww0d dddd ssss suuu uuuu uuuu                           "; ld%s{w} r%i{d}, 0x%x{u}(r%i{s})"
//1010 0010 ww1d dddd ssss suuu uuuu uuuu                           "; st%s{w} r%i{d}, 0x%x{u}(r%i{s})"
//1110 0110 ww0d dddd ssss sooo oooo oooo oooo oooo oooo oooo       "; ld%s{w} r%i{d}, 0x%08x{o}(r%i{s})"
//1110 0110 ww1d dddd ssss sooo oooo oooo oooo oooo oooo oooo       "; st%s{w} r%i{d}, 0x%08x{o}(r%i{s})"

    dat_reg=adreg(dat_reg);
    add_reg=adreg(add_reg);
    if(pass==0)
    {
        if(offset==0)
        {
            if((dat_reg>15)||(add_reg>15))
            {
                output[doff][0]=2;
                output[doff][1]=0xA200|(dat_reg);
                output[doff][2]=add_reg<<11;
            }
            else
            {
                output[doff][0]=1;
                output[doff][1]=0x0800|(add_reg<<4)|(dat_reg);
            }
        }
        else
        {
            if((dat_reg>15)||(add_reg>15))
            {
                if((offset&0x7FF)==offset)
                {
                    output[doff][0]=2;
                    output[doff][1]=0xA200|dat_reg;
                    output[doff][2]=(add_reg<<11)|offset;
                }
                else
                {
                }
            }
            else
            {
                if((offset&0x3C)==offset)
                {
                    output[doff][0]=1;
                    output[doff][1]=0x2000|(offset<<(8-2))|(add_reg<<4)|(dat_reg);
                }
                else
                {

                }
            }
        }

    }
    show_output();
}
//-------------------------------------------------------------------
void str ( unsigned int dat_reg, unsigned int add_reg, unsigned int offset )
{
//0000 1000 ssss dddd                                               "; ld%s{w} r%i{d}, (r%i{s})"
//0000 1001 ssss dddd                                               "; st%s{w} r%i{d}, (r%i{s})"
//0010 uuuu ssss dddd                                               "; ld  r%i{d}, 0x%02x{u*4}(r%i{s})"
//0011 uuuu ssss dddd                                               "; st  r%i{d}, 0x%02x{u*4}(r%i{s})"
//1010 0010 ww0d dddd ssss suuu uuuu uuuu                           "; ld%s{w} r%i{d}, 0x%x{u}(r%i{s})"
//1010 0010 ww1d dddd ssss suuu uuuu uuuu                           "; st%s{w} r%i{d}, 0x%x{u}(r%i{s})"
//1110 0110 ww0d dddd ssss sooo oooo oooo oooo oooo oooo oooo       "; ld%s{w} r%i{d}, 0x%08x{o}(r%i{s})"
//1110 0110 ww1d dddd ssss sooo oooo oooo oooo oooo oooo oooo       "; st%s{w} r%i{d}, 0x%08x{o}(r%i{s})"
    dat_reg=adreg(dat_reg);
    add_reg=adreg(add_reg);
    if(pass==0)
    {
        if(offset==0)
        {
            if((dat_reg>15)||(add_reg>15))
            {
                output[doff][0]=2;
                output[doff][1]=0xA220|(dat_reg);
                output[doff][2]=add_reg<<11;
            }
            else
            {
                output[doff][0]=1;
                output[doff][1]=0x0900|(add_reg<<4)|(dat_reg);
            }
        }
        else
        {
            if((dat_reg>15)||(add_reg>15))
            {
                if((offset&0x7FF)==offset)
                {
                    output[doff][0]=2;
                    output[doff][1]=0xA220|dat_reg;
                    output[doff][2]=(add_reg<<11)|offset;
                }
                else
                {
                }
            }
            else
            {
                if((offset&0x3C)==offset)
                {
                    output[doff][0]=1;
                    output[doff][1]=0x3000|(offset<<(8-2))|(add_reg<<4)|(dat_reg);
                }
                else
                {

                }
            }
        }

    }
    show_output();
}
//-------------------------------------------------------------------
void sub1u ( unsigned int rd, unsigned int u )
{
    //1011 00pp pppd dddd  iiii iiii iiii iiii                           "; %s{p} r%i{d}, #0x%04x{i}"
    if((u&0x1F)==u)
    {
        //011q qqqu uuuu dddd                                               "; %s{q} r%i{d}, #%i{u}"
        output[doff][0]=1;
        output[doff][1]=0x6000|(Q_SUB<<9)|(u<<4)|rd;
    }
    else
    {
        //1011 00pp pppd dddd  iiii iiii iiii iiii                           "; %s{p} r%i{d}, #0x%04x{i}"
        output[doff][0]=2;
        output[doff][1]=0xB000|(P_SUB<<5)|rd;
        output[doff][2]=u;
    }
    show_output();
}
//-------------------------------------------------------------------
void cmp1u ( unsigned int rd, unsigned int u )
{
    //1011 00pp pppd dddd  iiii iiii iiii iiii                           "; %s{p} r%i{d}, #0x%04x{i}"
    if((u&0x1F)==u)
    {
        //011q qqqu uuuu dddd                                               "; %s{q} r%i{d}, #%i{u}"
        output[doff][0]=1;
        output[doff][1]=0x6000|(Q_CMP<<9)|(u<<4)|rd;
    }
    else
    {
        //1011 00pp pppd dddd  iiii iiii iiii iiii                           "; %s{p} r%i{d}, #0x%04x{i}"
        output[doff][0]=2;
        output[doff][1]=0xB000|(P_CMP<<5)|rd;
        output[doff][2]=u;
    }
    show_output();
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void branchdest ( unsigned int addr )
{
    unsigned int doff;

    doff=addr2doff(addr);
if(doff>=MAXDATA)
{
    fprintf(stderr,"branchdest too big 0x%08X\n",addr);
    return;
}

    if(branch[doff]) return;
    branch[doff]=1;
//    fprintf(stderr,"branchdest(0x%08X)\n",addr);
}
//-------------------------------------------------------------------
void addhit ( unsigned int addr )
{
    unsigned int doff;

    doff=addr2doff(addr);
if(doff>=MAXDATA)
{
    fprintf(stderr,"addhit too big 0x%08X\n",addr);
    return;
}
    if(hit[doff]) return;
    hit[doff]=1;
    newhit=1;
//    fprintf(stderr,"addhit(0x%08X)\n",addr);
}
//-------------------------------------------------------------------
void addokay ( unsigned int addr )
{
    unsigned int doff;

    doff=addr2doff(addr);
if(doff>=MAXDATA)
{
    fprintf(stderr,"addokay too big 0x%08X\n",addr);
    return;
}
    if(okay[doff]) return;
    okay[doff]=1;
    //fprintf(stderr,"addokay(0x%08X)\n",addr);
}
//-------------------------------------------------------------------
int main ( int argc, char *argv[] )
{
    unsigned int datalen;
    unsigned int halfadd;
    unsigned int inst;
    unsigned int ra,rb,rd,rn;
    unsigned int rm,rs,rc;
    unsigned int addr;
    unsigned int op;
    unsigned int run;


    FILE *fp;

    if(argc<2)
    {
        fprintf(stderr,".bin file not specified\n");
        return(1);
    }
    fp=fopen(argv[1],"rb");
    if(fp==NULL)
    {
        fprintf(stderr,"Error opening file [%s]\n",argv[1]);
        return(1);
    }
    memset(data,0,sizeof(data));
    datalen=fread(data,1,sizeof(data),fp);
    fclose(fp);
    datalen=(datalen+1)>>1;
    fprintf(stderr,"datalen %u\n",datalen);
    if(datalen==0)
    {
        fprintf(stderr,"empty file\n");
        return(1);
    }

    memset(hit,0,sizeof(hit));
    memset(okay,0,sizeof(okay));
    memset(branch,0,sizeof(branch));
    memset(output,0,sizeof(output));

    addhit(0);
    while(1)
    {
        newhit=0;
        for(doff=0;doff<datalen;doff++)
        {
            if(hit[doff]==0) continue;
            inst=data[doff];
            addr=doff2addr(doff);
            run=1;

            //B(1) conditional branch
            if((inst&0xF000)==0xD000)
            {
                rb=(inst>>0)&0xFF;
                if(rb&0x80) rb|=(~0)<<8;
                op=(inst>>8)&0xF;
                rb<<=1;
                rb+=addr;
                rb+=4;
                if(op<0xE)
                {
                    branchdest(rb);
                    addhit(rb);
                }
            }

            //B(2) unconditional branch
            if((inst&0xF800)==0xE000)
            {
                rb=(inst>>0)&0x7FF;
                if(rb&(1<<10)) rb|=(~0)<<11;
                rb<<=1;
                rb+=addr;
                rb+=4;
                branchdest(rb);
                addhit(rb);
                run=0;
            }


            //BL/BLX(1)
            if((inst&0xE000)==0xE000) //BL,BLX
            {
                if((inst&0x1800)==0x1000) //H=b10
                {
                    halfadd=inst;
                    addokay(addr);
                }
                else
                if((inst&0x1800)==0x1800) //H=b11
                {
                    //branch to thumb
                    rb=halfadd&((1<<11)-1);
                    if(rb&1<<10) rb|=(~((1<<11)-1)); //sign extend
                    rb<<=11;
                    rb|=inst&((1<<11)-1);
                    rb<<=1;
                    rb+=addr;
                    rb+=2;
                    branchdest(rb);
                    addhit(rb);
                }
            }


            //BX
            if((inst&0xFF87)==0x4700)
            {
                run=0;
            }
            if(run) addhit((doff+1)<<1);

        }
        if(newhit==0) break;
    }

    for(pass=0;pass<2;pass++)
    {
        printf("// ----- pass %u -----\n",pass);
        for(doff=0;doff<datalen;doff++)
        {
            if(hit[doff]==0) continue;
            inst=data[doff];
            addr=doff2addr(doff);

            printf("\n//[0x%06X] 0x%04X  ",addr,inst);

            //ADC
            if((inst&0xFFC0)==0x4140)
            {
                rd=(inst>>0)&0x07;
                rm=(inst>>3)&0x07;
                printf("adc r%u,r%u\n",rd,rm);
                continue;
            }

            //ADD(1) small immediate two registers
            if((inst&0xFE00)==0x1C00)
            {
                rd=(inst>>0)&0x7;
                rn=(inst>>3)&0x7;
                rb=(inst>>6)&0x7;
                if(rb)
                {
                    printf("adds r%u,r%u,#0x%X\n",rd,rn,rb);
                    continue;
                }
                else
                {
                    //this is a mov
                }
            }

            //ADD(2) big immediate one register
            if((inst&0xF800)==0x3000)
            {
                rb=(inst>>0)&0xFF;
                rd=(inst>>8)&0x7;
                printf("adds r%u,#0x%02X\n",rd,rb);
                pc2u(P_ADD,CC_UN,rd,rd,rb);
                continue;
            }

            //ADD(3) three registers
            if((inst&0xFE00)==0x1800)
            {
                rd=(inst>>0)&0x7;
                rn=(inst>>3)&0x7;
                rm=(inst>>6)&0x7;
                printf("adds r%u,r%u,r%u\n",rd,rn,rm);
                pc3(P_ADD,CC_UN,rd,rn,rm);
                continue;
            }

            //ADD(4) two registers one or both high no flags
            if((inst&0xFF00)==0x4400)
            {
                if((inst>>6)&3)
                {
                    //UNPREDICTABLE
                }
                rd=(inst>>0)&0x7;
                rd|=(inst>>4)&0x8;
                rm=(inst>>3)&0xF;
                printf("add r%u,r%u\n",rd,rm);
                pc3(P_ADD,CC_UN,rd,rd,rm);
                continue;
            }

            //ADD(5) rd = pc plus immediate
            if((inst&0xF800)==0xA000)
            {
                rb=(inst>>0)&0xFF;
                rd=(inst>>8)&0x7;
                rb<<=2;
                printf("add r%u,PC,#0x%02X\n",rd,rb);
                continue;
            }

            //ADD(6) rd = sp plus immediate
            if((inst&0xF800)==0xA800)
            {
                rb=(inst>>0)&0xFF;
                rd=(inst>>8)&0x7;
                rb<<=2;
                printf("add r%u,SP,#0x%02X\n",rd,rb);
                continue;
            }

            //ADD(7) sp plus immediate
            if((inst&0xFF80)==0xB000)
            {
                rb=(inst>>0)&0x7F;
                rb<<=2;
                printf("add SP,#0x%02X\n",rb);
                continue;
            }

            //AND
            if((inst&0xFFC0)==0x4000)
            {
                rd=(inst>>0)&0x7;
                rm=(inst>>3)&0x7;
                printf("ands r%u,r%u\n",rd,rm);
                pc3(P_AND,CC_UN,rd,rd,rm);
                continue;
            }

            //ASR(1) two register immediate
            if((inst&0xF800)==0x1000)
            {
                rd=(inst>>0)&0x07;
                rm=(inst>>3)&0x07;
                rb=(inst>>6)&0x1F;
                printf("asrs r%u,r%u,#0x%X\n",rd,rm,rb);
                continue;
            }

            //ASR(2) two register
            if((inst&0xFFC0)==0x4100)
            {
                rd=(inst>>0)&0x07;
                rs=(inst>>3)&0x07;
                printf("asrs r%u,r%u\n",rd,rs);
                continue;
            }
            //B(1) conditional branch
            if((inst&0xF000)==0xD000)
            {
                rb=(inst>>0)&0xFF;
                if(rb&0x80) rb|=(~0)<<8;
                op=(inst>>8)&0xF;
                rb<<=1;
                rb+=addr;
                rb+=4;
                if(op<0xE)
                {
                    switch(op)
                    {
                        case 0x0: //b eq  z set
                        {
                            printf("beq 0x%08X\n",rb);
                            bxx(CC_EQ,rb);
                            break;
                        }
                        case 0x1: //b ne  z clear
                        {
                            printf("bne 0x%08X\n",rb);
                            bxx(CC_NE,rb);
                            break;
                        }
                        case 0x2: //b cs c set
                        {
                            printf("bcs 0x%08X\n",rb);
                            break;
                        }
                        case 0x3: //b cc c clear
                        {
                            printf("bcc 0x%08X\n",rb);
                            break;
                        }

                        case 0x4: //b mi n set
                        {
                            printf("bmi 0x%08X\n",rb);
                            bxx(CC_MI,rb);
                            break;
                        }
                        case 0x5: //b pl n clear
                        {
                            printf("bpl 0x%08X\n",rb);
                            bxx(CC_PL,rb);
                            break;
                        }
                        case 0x6: //b vs v set
                        {
                            printf("bvs 0x%08X\n",rb);
                            bxx(CC_VS,rb);
                            break;
                        }
                        case 0x7: //b vc v clear
                        {
                            printf("bvc 0x%08X\n",rb);
                            bxx(CC_VC,rb);
                            break;
                        }
                        case 0x8: //b hi c set z clear
                        {
                            printf("bhi 0x%08X\n",rb);
                            bxx(CC_HI,rb);
                            break;
                        }
                        case 0x9: //b ls c clear or z set
                        {
                            printf("bls 0x%08X\n",rb);
                            bxx(CC_LS,rb);
                            break;
                        }

                        case 0xA: //b ge N == V
                        {
                            printf("bge 0x%08X\n",rb);
                            bxx(CC_GE,rb);
                            break;
                        }

                        case 0xB: //b lt N != V
                        {
                            printf("blt 0x%08X\n",rb);
                            bxx(CC_LT,rb);
                            break;
                        }

                        case 0xC: //b gt Z==0 and N == V
                        {
                            printf("bgt 0x%08X\n",rb);
                            bxx(CC_GT,rb);
                            break;
                        }

                        case 0xD: //b le Z==1 or N != V
                        {
                            printf("ble 0x%08X\n",rb);
                            bxx(CC_LE,rb);
                            break;
                        }
                    }
                    continue;
                }
            }

            //B(2) unconditional branch
            if((inst&0xF800)==0xE000)
            {
                rb=(inst>>0)&0x7FF;
                if(rb&(1<<10)) rb|=(~0)<<11;
                rb<<=1;
                rb+=addr;
                rb+=4;
                printf("b 0x%05X\n",rb);
                b(rb);
                continue;
            }

            //BL/BLX(1)
            if((inst&0xE000)==0xE000) //BL,BLX
            {
                if((inst&0x1800)==0x1000) //H=b10
                {
                    halfadd=inst;
                    okay[doff]=1;
                    printf("bl halfadd\n");
                    continue;
                }
                else
                if((inst&0x1800)==0x1800) //H=b11
                {
                    //branch to thumb
                    rb=halfadd&((1<<11)-1);
                    if(rb&1<<10) rb|=(~((1<<11)-1)); //sign extend
                    rb<<=11;
                    rb|=inst&((1<<11)-1);
                    rb<<=1;
                    rb+=addr;
                    rb+=2;
                    printf("bl 0x%05X\n",rb);
                    bl(rb);
                    continue;
                }
            }


            //BX
            if((inst&0xFF87)==0x4700)
            {
                rm=(inst>>3)&0xF;
                printf("bx r%u\n",rm);
                if(rm==14)
                {
                    rts();
                }
                else
                {
                    br(rm);
                }
                continue;
            }


            //CMN
            if((inst&0xFFC0)==0x42C0)
            {
                rn=(inst>>0)&0x7;
                rm=(inst>>3)&0x7;
                printf("cmns r%u,r%u\n",rn,rm);
                continue;
            }

            //CMP(1) compare immediate
            if((inst&0xF800)==0x2800)
            {
                rb=(inst>>0)&0xFF;
                rn=(inst>>8)&0x07;
                printf("cmp r%u,#0x%02X\n",rn,rb);
                cmp1u(rn,rb);
                continue;
            }

            //CMP(2) compare register
            if((inst&0xFFC0)==0x4280)
            {
                rn=(inst>>0)&0x7;
                rm=(inst>>3)&0x7;
                printf("cmps r%u,r%u\n",rn,rm);
                pc3(P_CMP,CC_UN,rn,rn,rm);
                continue;
            }

            //CMP(3) compare high register
            if((inst&0xFF00)==0x4500)
            {
                if(((inst>>6)&3)==0x0)
                {
                    //UNPREDICTABLE
                }
                rn=(inst>>0)&0x7;
                rn|=(inst>>4)&0x8;
                if(rn==0xF)
                {
                    //UNPREDICTABLE
                }
                rm=(inst>>3)&0xF;
                printf("cmps r%u,r%u\n",rn,rm);
                pc3(P_CMP,CC_UN,rn,rn,rm);
                continue;
            }



            //BIC
            if((inst&0xFFC0)==0x4380)
            {
                rd=(inst>>0)&0x7;
                rm=(inst>>3)&0x7;
                printf("bics r%u,r%u\n",rd,rm);
                continue;
            }




            //LDR(1) two register immediate
            if((inst&0xF800)==0x6800)
            {
                rd=(inst>>0)&0x07;
                rn=(inst>>3)&0x07;
                rb=(inst>>6)&0x1F;
                rb<<=2;
                printf("ldr r%u,[r%u,#0x%X]\n",rd,rn,rb);
                ldr(rd,rn,rb);
                continue;
            }

            //LDR(2) three register
            if((inst&0xFE00)==0x5800)
            {
                rd=(inst>>0)&0x7;
                rn=(inst>>3)&0x7;
                rm=(inst>>6)&0x7;
                printf("ldr r%u,[r%u,r%u]\n",rd,rn,rm);
                continue;
            }

            //LDR(3)
            if((inst&0xF800)==0x4800)
            {
                rb=(inst>>0)&0xFF;
                rd=(inst>>8)&0x07;
                rb<<=2;
                printf("ldr r%u,[PC+#0x%X] ",rd,rb);
                ra=addr+4;
                ra&=~3;
                rb+=ra;
                printf(";@ 0x%X ",rb);

                rc =data[(rb>>1)+1];
                rc<<=16;
                rc|=data[(rb>>1)+0];
                printf("(0x%08X)\n",rc);
                movi(rd,rc);
                continue;
            }

            //LDR(4)
            if((inst&0xF800)==0x9800)
            {
                rb=(inst>>0)&0xFF;
                rd=(inst>>8)&0x07;
                rb<<=2;
                printf("ldr r%u,[SP+#0x%X]\n",rd,rb);
                continue;
            }










            //STR(1)
            if((inst&0xF800)==0x6000)
            {
                rd=(inst>>0)&0x07;
                rn=(inst>>3)&0x07;
                rb=(inst>>6)&0x1F;
                rb<<=2;
                printf("str r%u,[r%u,#0x%X]\n",rd,rn,rb);
                str(rd,rn,rb);
                continue;
            }

            //STR(2)
            if((inst&0xFE00)==0x5000)
            {
                rd=(inst>>0)&0x7;
                rn=(inst>>3)&0x7;
                rm=(inst>>6)&0x7;
                printf("str r%u,[r%u,r%u]\n",rd,rn,rm);
                continue;
            }

            //STR(3)
            if((inst&0xF800)==0x9000)
            {
                rb=(inst>>0)&0xFF;
                rd=(inst>>8)&0x07;
                rb<<=2;
                printf("str r%u,[SP,#0x%X]\n",rd,rb);
                continue;
            }





            //EOR
            if((inst&0xFFC0)==0x4040)
            {
                rd=(inst>>0)&0x7;
                rm=(inst>>3)&0x7;
                printf("eors r%u,r%u\n",rd,rm);
                pc3(P_EOR,CC_UN,rd,rd,rm);
                continue;
            }



            //LSL(1)
            if((inst&0xF800)==0x0000)
            {
                rd=(inst>>0)&0x07;
                rm=(inst>>3)&0x07;
                rb=(inst>>6)&0x1F;
                printf("lsls r%u,r%u,#0x%X\n",rd,rm,rb);
                pc2u(P_LSL,CC_UN,rd,rm,rb);
                continue;
            }

            //LSL(2) two register
            if((inst&0xFFC0)==0x4080)
            {
                rd=(inst>>0)&0x07;
                rs=(inst>>3)&0x07;
                printf("lsls r%u,r%u\n",rd,rs);
                pc3(P_LSL,CC_UN,rd,rd,rs);
                continue;
            }

            //LSR(1) two register immediate
            if((inst&0xF800)==0x0800)
            {
                rd=(inst>>0)&0x07;
                rm=(inst>>3)&0x07;
                rb=(inst>>6)&0x1F;
                printf("lsrs r%u,r%u,#0x%X\n",rd,rm,rb);
                continue;
            }

            //LSR(2) two register
            if((inst&0xFFC0)==0x40C0)
            {
                rd=(inst>>0)&0x07;
                rs=(inst>>3)&0x07;
                printf("lsrs r%u,r%u\n",rd,rs);
                pc3(P_LSR,CC_UN,rd,rd,rs);
                continue;
            }

            //MOV(1) immediate
            if((inst&0xF800)==0x2000)
            {
                rb=(inst>>0)&0xFF;
                rd=(inst>>8)&0x07;
                printf("movs r%u,#0x%02X\n",rd,rb);
                movi(rd,rb);
                continue;
            }
            //MOV(2) two low registers
            if((inst&0xFFC0)==0x1C00)
            {
                rd=(inst>>0)&7;
                rn=(inst>>3)&7;
                printf("movs r%u,r%u\n",rd,rn);
                pc3(P_MOV,CC_UN,rd,rd,rn);
                continue;
            }
            //MOV(3)
            if((inst&0xFF00)==0x4600)
            {
                rd=(inst>>0)&0x7;
                rd|=(inst>>4)&0x8;
                rm=(inst>>3)&0xF;
                printf("mov r%u,r%u\n",rd,rm);
                pc3(P_MOV,CC_UN,rd,rm,rm);
                continue;
            }
            //MVN
            if((inst&0xFFC0)==0x43C0)
            {
                rd=(inst>>0)&0x7;
                rm=(inst>>3)&0x7;
                printf("mvns r%u,r%u\n",rd,rm);
                pc3(P_SUB,CC_UN,rd,rm,rm);
                continue;
            }

            //NEG
            if((inst&0xFFC0)==0x4240)
            {
                rd=(inst>>0)&0x7;
                rm=(inst>>3)&0x7;
                printf("negs r%u,r%u\n",rd,rm);
                continue;
            }

            //ORR
            if((inst&0xFFC0)==0x4300)
            {
                rd=(inst>>0)&0x7;
                rm=(inst>>3)&0x7;
                printf("orrs r%u,r%u\n",rd,rm);
                pc3(P_OR,CC_UN,rd,rd,rm);
                continue;
            }




            //ROR
            if((inst&0xFFC0)==0x41C0)
            {
                rd=(inst>>0)&0x7;
                rs=(inst>>3)&0x7;
                printf("rors r%u,r%u\n",rd,rs);
                continue;
            }

            //SBC
            if((inst&0xFFC0)==0x4180)
            {
                rd=(inst>>0)&0x7;
                rm=(inst>>3)&0x7;
                printf("sbc r%u,r%u\n",rd,rm);
                continue;
            }



            //SUB(1)
            if((inst&0xFE00)==0x1E00)
            {
                rd=(inst>>0)&7;
                rn=(inst>>3)&7;
                rb=(inst>>6)&7;
                printf("subs r%u,r%u,#0x%X\n",rd,rn,rb);
                pc2u(P_SUB,CC_UN,rd,rn,rb);
                continue;
            }

            //SUB(2)
            if((inst&0xF800)==0x3800)
            {
                rb=(inst>>0)&0xFF;
                rd=(inst>>8)&0x07;
                printf("subs r%u,#0x%02X\n",rd,rb);
                sub1u(rd,rb);
                continue;
            }

            //SUB(3)
            if((inst&0xFE00)==0x1A00)
            {
                rd=(inst>>0)&0x7;
                rn=(inst>>3)&0x7;
                rm=(inst>>6)&0x7;
                printf("subs r%u,r%u,r%u\n",rd,rn,rm);
                pc3(P_SUB,CC_UN,rd,rn,rm);
                continue;
            }

            //SUB(4)
            if((inst&0xFF80)==0xB080)
            {
                rb=inst&0x7F;
                rb<<=2;
                printf("sub SP,#0x%02X\n",rb);
                pc2u(P_SUB,CC_UN,13,13,rb);
                continue;
            }

            //TST
            if((inst&0xFFC0)==0x4200)
            {
                rn=(inst>>0)&0x7;
                rm=(inst>>3)&0x7;
                printf("tst r%u,r%u\n",rn,rm);
                tst(rn,rm);
                continue;
            }




            //POP
            if((inst&0xFE00)==0xBC00)
            {
                printf("pop {");
                for(ra=0,rb=0x01,rc=0;rb;rb=(rb<<1)&0xFF,ra++)
                {
                    if(inst&rb)
                    {
                        if(rc) printf(",");
                        printf("r%u",ra);
                        rc++;
                    }
                }
                if(inst&0x100)
                {
                    if(rc) printf(",");
                    printf("pc");
                }
                printf("}\n");

                output[doff][0]=0;
                for(ra=0,rb=0x01,rc=0;rb;rb=(rb<<1)&0xFF,ra++)
                {
                    if(inst&rb)
                    {
                        pop(ra);
                    }
                }
                if(inst&0x100)
                {
                    //pop pc
                }
                show_output();
                continue;
            }



            //PUSH
            if((inst&0xFE00)==0xB400)
            {

                printf("push {");
                for(ra=0,rb=0x01,rc=0;rb;rb=(rb<<1)&0xFF,ra++)
                {
                    if(inst&rb)
                    {
                        if(rc) printf(",");
                        printf("r%u",ra);
                        rc++;
                    }
                }
                if(inst&0x100)
                {
                    if(rc) printf(",");
                    printf("lr");
                }
                printf("}\n");

                output[doff][0]=0;
                if(inst&0x100)
                {
                    //pushl();
                    push(26);
                }
                for(ra=7,rb=0x80;rb;rb=rb>>1,ra--)
                {
                    if(inst&rb)
                    {
                        push(ra);
                    }
                }
                show_output();
                continue;
            }



            printf("UNKNOWN\n");
        }

    }

    for(doff=0;doff<datalen;doff++)
    {
        if(hit[doff]==0) continue;
        if(okay[doff]==1) continue;
        addr=doff2addr(doff);
        if(output[doff][0]==0)
        {
            printf("[0x%08X] no output\n",addr);
            fprintf(stderr,"[0x%08X] no output\n",addr);
        }
    }




    return(0);
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------


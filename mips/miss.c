
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXDATA 0x1000

unsigned int data[MAXDATA];
unsigned short output[MAXDATA][32];
unsigned char hit[MAXDATA];
unsigned char branch[MAXDATA];

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

char q_name[0x10][8]=
{
"mov",
"add",
"mul",
"sub",
"mvn",
"cmp",
"btst",
"extu",
"bset",
"bclr",
"bchg",
"adds8",
"exts",
"lsr",
"lsl",
"asr"
};



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
    switch(rx)
    {
        case 31:
        {
            printf("//changing r31, to r29\n");
            rx=29;
            break;
        }
        case 29:
        {
            printf("//changing r29, to r25\n");
            rx=25;
            break;
        }
    }
    return(rx);
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
void diss_data ( void )
{
    unsigned int x;
    unsigned int inst;

    for(x=1;x<=output[doff][0];x++,dissadd+=2)
    {
        inst=output[doff][x];
        printf("//<0x%06X> 0x%04X DATA\n",dissadd,inst);
    }
}
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
void data_word ( unsigned int data )
{
    if(pass==0)
    {
        output[doff][0]=2;
        output[doff][1]=(data>>16)&0xFFFF;
        output[doff][2]=data&0xFFFF;
    }
    show_output();
}
//-------------------------------------------------------------------
unsigned int build_p5d32u ( unsigned int p, unsigned int rd, unsigned int u, unsigned short *out )
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
unsigned int build_q4d5u ( unsigned int q, unsigned int rd, unsigned int u, unsigned short *out)
{
    //011q qqqu uuuu dddd                                               "; %s{q} r%i{d}, #%i{u}"
    q&=0xF;
    rd&=0xF;
    u&=0x1F;
    out[0]=0x6000|(q<<9)|(u<<4)|rd;
    return(1);
}//-------------------------------------------------------------------
void nop ( void )
{
    if(pass==0)
    {
        output[doff][0]=1;
        output[doff][1]=0x0001;
    }
    show_output();
}
//-------------------------------------------------------------------
void sltu ( unsigned int rd, unsigned int rs, unsigned int rt)
{
    rd=adreg(rd);
    rs=adreg(rs);
    rt=adreg(rt);

    if(pass==0)
    {
        if((rd>15)||(rs>15)||(rt>15))
        {
            if(rd>15)
            {
            }
            else
            {
                //cmp(rs,rt);
                //movi(rd,0);
                //addltu(rd,1);
                output[doff][0]=5;
                //1100 00pp pppd dddd   aaaa accc c00b bbbb    "; %s{p}%s{c} r%i{d}, r%i{a}, r%i{b}"
                output[doff][1]=0xC000|(P_CMP<<5)|rd;
                output[doff][2]=0x0000|(rs<<11)|(0xE<<7)|(rt);
                //# op rd, #u   (rd = rd op #u)
                //011q qqqu uuuu dddd                            "; %s{q} r%i{d}, #%i{u}"
                output[doff][3]=0x6000|(Q_MOV<<9)|(0<<4)|rd;
                //1100 00pp pppd dddd  aaaa accc c1ii iiii       "; %s{p}%s{c} r%i{d}, r%i{a}, #%i{i}"
                output[doff][4]=0xC000|(P_ADD<<5)|rd;
                output[doff][5]=0x0040|(rd<<11)|(CC_LO<<7)|1;
            }
        }
        else
        {
            //cmp(rs,rt);
            //movi(rd,0);
            //addltu(rd,1);
            output[doff][0]=4;
            //010p pppp ssss dddd             "; %s{p} r%i{d}, r%i{s}"
            output[doff][1]=0x4000|(P_CMP<<8)|(rt<<4)|(rs);
            //# op rd, #u   (rd = rd op #u)
            //011q qqqu uuuu dddd                                "; %s{q} r%i{d}, #%i{u}"
            output[doff][2]=0x6000|(Q_MOV<<9)|(0<<4)|rd;
            //1100 00pp pppd dddd  aaaa accc c1ii iiii           "; %s{p}%s{c} r%i{d}, r%i{a}, #%i{i}"
            output[doff][3]=0xC000|(P_ADD<<5)|rd;
            output[doff][4]=0x0040|(rd<<11)|(CC_LO<<6)|1;
        }
    }
    show_output();
}
//-------------------------------------------------------------------
void cmp_bne ( unsigned int rs, unsigned int rt, unsigned int address)
{
    unsigned int ra,rb;

    rs=adreg(rs);
    rt=adreg(rt);
    if((rs>15)||(rt>15))
    {
        output[doff][0]=4;
        //1100 00pp pppd dddd   aaaa accc c00b bbbb   "; %s{p}%s{c} r%i{d}, r%i{a}, r%i{b}"
        output[doff][1]=0xC000|(P_CMP<<5)|0;
        output[doff][2]=0x0000|(rs<<11)|(CC_UN<<7)|rt;
        //1001 cccc 0ooo oooo   oooo oooo oooo oooo    "; b%s{c} 0x%08x{$+o*2}"
        output[doff][3]=0x9000|(CC_NE<<8);
        output[doff][4]=0x0000;
        if(pass)
        {
            if(address==addr)
            {
                printf("x\n");
            }
            else
            if(address<addr)
            {
                rb=0;
rb=2;
                for(ra=(address>>2);ra<doff;ra++)
                {
                    if(hit[ra]&&output[ra][0]==0)
                    {
                        printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",ra<<2,addr);
                        rb=0;
                        break;
                    }
                    rb+=output[ra][0];
                }
                if(rb)
                {
                    printf("//bne backward %u 0x%X\n",rb,rb);
                    rb=-rb;
                    output[doff][3]|=(rb>>16)&0x7F;
                    output[doff][4]|=rb&0xFFFF;
                }
            }
            else
            {
                printf("x\n");
            }
        }
    }
    else
    {
        output[doff][0]=3;
        //010p pppp ssss dddd             "; %s{p} r%i{d}, r%i{s}"
        output[doff][1]=0x4000|(P_CMP<<8)|(rt<<4)|(rs);
        //1001 cccc 0ooo oooo   oooo oooo oooo oooo    "; b%s{c} 0x%08x{$+o*2}"
        output[doff][2]=0x9000|(CC_NE<<8);
        output[doff][3]=0x0000;
        if(pass)
        {
            if(address==addr)
            {
                printf("x\n");
            }
            else
            if(address<addr)
            {
                rb=0;
rb=1;
                for(ra=(address>>2);ra<doff;ra++)
                {
                    if(hit[ra]&&output[ra][0]==0)
                    {
                        printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",ra<<2,addr);
                        rb=0;
                        break;
                    }
                    rb+=output[ra][0];
                }
                if(rb)
                {
                    printf("//bne backward %u 0x%X\n",rb,rb);
                    rb=-rb;
                    output[doff][2]|=(rb>>16)&0x7F;
                    output[doff][3]|=rb&0xFFFF;
                }
            }
            else
            {
                printf("x\n");
            }
        }
    }
    show_output();
}
//-------------------------------------------------------------------
//-------------------------------------------------------------------
void cmp_beq ( unsigned int rs, unsigned int rt, unsigned int address)
{
    unsigned int ra,rb;

    rs=adreg(rs);
    rt=adreg(rt);
    if((rs>15)||(rt>15))
    {
        output[doff][0]=4;
        //1100 00pp pppd dddd   aaaa accc c00b bbbb   "; %s{p}%s{c} r%i{d}, r%i{a}, r%i{b}"
        output[doff][1]=0xC000|(P_CMP<<5)|0;
        output[doff][2]=0x0000|(rs<<11)|(CC_UN<<7)|rt;
        //1001 cccc 0ooo oooo   oooo oooo oooo oooo    "; b%s{c} 0x%08x{$+o*2}"
        output[doff][3]=0x9000|(CC_EQ<<8);
        output[doff][4]=0x0000;
        if(pass)
        {
            if(address==addr)
            {
                printf("x\n");
            }
            else
            if(address<addr)
            {
                rb=0;
rb=2;
                for(ra=(address>>2);ra<doff;ra++)
                {
                    if(hit[ra]&&output[ra][0]==0)
                    {
                        printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",ra<<2,addr);
                        rb=0;
                        break;
                    }
                    rb+=output[ra][0];
                }
                if(rb)
                {
                    printf("//beq backward %u 0x%X\n",rb,rb);
                    rb=-rb;
                    output[doff][3]|=(rb>>16)&0x7F;
                    output[doff][4]|=rb&0xFFFF;
                }
            }
            else
            {
                printf("x\n");
            }
        }
    }
    else
    {
        output[doff][0]=3;
        //010p pppp ssss dddd             "; %s{p} r%i{d}, r%i{s}"
        output[doff][1]=0x4000|(P_CMP<<8)|(rt<<4)|(rs);
        //1001 cccc 0ooo oooo   oooo oooo oooo oooo    "; b%s{c} 0x%08x{$+o*2}"
        output[doff][2]=0x9000|(CC_EQ<<8);
        output[doff][3]=0x0000;
        if(pass)
        {
            if(address==addr)
            {
                printf("x\n");
            }
            else
            if(address<addr)
            {
                rb=0;
rb=1;
                for(ra=(address>>2);ra<doff;ra++)
                {
                    if(hit[ra]&&output[ra][0]==0)
                    {
                        printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",ra<<2,addr);
                        rb=0;
                        break;
                    }
                    rb+=output[ra][0];
                }
                if(rb)
                {
                    printf("//beq backward %u 0x%X\n",rb,rb);
                    rb=-rb;
                    output[doff][2]|=(rb>>16)&0x7F;
                    output[doff][3]|=rb&0xFFFF;
                }
            }
            else
            {
                printf("x\n");
            }
        }
    }
    show_output();
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

    if((rd==ra)&&(cc==CC_UN)&&(rd<16)&&(rb<16))
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
    if(((u&0xFFFFFFE0)==0)&&(rd<16)&&(cc==CC_UN)&&(q<Q_SIZE)&&(rd==ra))
    {
        //011q qqqu uuuu dddd                                               "; %s{q} r%i{d}, #%i{u}"
        output[doff][0]=1;
        output[doff][1]=0x6000|(q<<9)|(u<<4)|(rd);
    }
    else
    if(((u&0xFFFFFFE0)==0)||((u&0xFFFFFFE0)==0xFFFFFFE0))
    {
        //1100 00pp pppd dddd  aaaa accc c1ii iiii                           "; %s{p}%s{c} r%i{d}, r%i{a}, #%i{i}"
        output[doff][0]=2;
        output[doff][1]=0xC000|(p<<5)|rd;
        output[doff][2]=0x0040|(ra<<11)|(cc<<7)|(u&0x3F);
    }
    else
    if((((u&0xFFFF8000)==0)||((u&0xFFFF8000)==0xFFFF8000))&&(rd==ra)&&(cc==CC_UN))
    {
        //1011 00pp pppd dddd  iiii iiii iiii iiii                           "; %s{p} r%i{d}, #0x%04x{i}"
        output[doff][0]=2;
        output[doff][1]=0xB000|(p<<5)|rd;
        output[doff][2]=u;
    }
    else
    if((rd==ra)&&(cc==CC_UN))
    {
printf("//untested\n");
        output[doff][0]=3;
        //rd = rd op u
        //1110 10pp pppd dddd  uuuu uuuu uuuu uuuu  uuuu uuuu uuuu uuuu       "; %s{p} r%i{d}, #0x%08x{u}"
        build_p5d32u(p,rd,u,&output[doff][1]); //uses 3
        //output[doff][1]
        //output[doff][2]
        //output[doff][3]
    }
    else
    {
printf("//untested\n");
        output[doff][0]=5;
        //mov rd,u
        //rd = ra op rd
        //1110 10pp pppd dddd  uuuu uuuu uuuu uuuu  uuuu uuuu uuuu uuuu       "; %s{p} r%i{d}, #0x%08x{u}"
        build_p5d32u(P_MOV,rd,u,&output[doff][1]); //uses 3
        //output[doff][1]
        //output[doff][2]
        //output[doff][3]
        //1100 00pp pppd dddd  aaaa accc c00b bbbb                           "; %s{p}%s{c} r%i{d}, r%i{a}, r%i{b}"
        build_pc5dab(p,cc,rd,ra,rd,&output[doff][4]); //uses 2
        //output[doff][4]
        //output[doff][5]
    }
    show_output();
}



//-------------------------------------------------------------------
void and3 ( unsigned int rd, unsigned int ra, unsigned int rb)
{
//1100 00pp pppd dddd aaaa accc c00b bbbb
//010p pppp ssss dddd   "; %s{p} r%i{d}, r%i{s}"
    rd=adreg(rd);
    ra=adreg(ra);
    rb=adreg(rb);
    if(pass==0)
    {
        if((rd>15)||(ra>15)||(rb>15))
        {
            output[doff][0]=2;
            output[doff][1]=0xC000|(0x07<<5)|(rd);
            output[doff][2]=0x0000|(ra<<11)|(0xE<<7)|(rb);
        }
        else
        {
            if(rd==ra)
            {
                output[doff][0]=1;
                output[doff][1]=0x4000|(0x07<<8)|(rb<<4)|rd;
            }
            else
            if(rd==rb)
            {
                output[doff][0]=1;
                output[doff][1]=0x4000|(0x07<<8)|(ra<<4)|rd;
            }
            else
            {
                output[doff][0]=2;
                output[doff][1]=0xC000|(0x07<<5)|(rd);
                output[doff][2]=0x0000|(ra<<11)|(0xE<<7)|(rb);
            }
        }
    }
    show_output();
}
//-------------------------------------------------------------------
void add3 ( unsigned int rd, unsigned int ra, unsigned int rb)
{
//1100 00pp pppd dddd aaaa accc c00b bbbb
//010p pppp ssss dddd   "; %s{p} r%i{d}, r%i{s}"
    rd=adreg(rd);
    ra=adreg(ra);
    rb=adreg(rb);
    if(pass==0)
    {
        if((rd>15)||(ra>15)||(rb>15))
        {
            output[doff][0]=2;
            output[doff][1]=0xC000|(0x02<<5)|(rd);
            output[doff][2]=0x0000|(ra<<11)|(0xE<<7)|(rb);
        }
        else
        {
            if(rd==ra)
            {
                output[doff][0]=1;
                output[doff][1]=0x4000|(0x02<<8)|(rb<<4)|rd;
            }
            else
            if(rd==rb)
            {
                output[doff][0]=1;
                output[doff][1]=0x4000|(0x02<<8)|(ra<<4)|rd;
            }
            else
            {
                output[doff][0]=2;
                output[doff][1]=0xC000|(0x02<<5)|(rd);
                output[doff][2]=0x0000|(ra<<11)|(0xE<<7)|(rb);
            }
        }
    }
    show_output();
}
//-------------------------------------------------------------------
void sub3 ( unsigned int rd, unsigned int ra, unsigned int rb)
{
//1100 00pp pppd dddd aaaa accc c00b bbbb
//010p pppp ssss dddd   "; %s{p} r%i{d}, r%i{s}"
    rd=adreg(rd);
    ra=adreg(ra);
    rb=adreg(rb);
    if(pass==0)
    {
        if((rd>15)||(ra>15)||(rb>15))
        {
            output[doff][0]=2;
            output[doff][1]=0xC000|(0x06<<5)|(rd);
            output[doff][2]=0x0000|(ra<<11)|(0xE<<7)|(rb);
        }
        else
        {
            if(rd==ra)
            {
                output[doff][0]=1;
                output[doff][1]=0x4000|(0x06<<8)|(rb<<4)|rd;
            }
            else
            if(rd==rb)
            {
                output[doff][0]=1;
                output[doff][1]=0x4000|(0x06<<8)|(ra<<4)|rd;
            }
            else
            {
                output[doff][0]=2;
                output[doff][1]=0xC000|(0x06<<5)|(rd);
                output[doff][2]=0x0000|(ra<<11)|(0xE<<7)|(rb);
            }
        }
    }
    show_output();
}
//-------------------------------------------------------------------
void or3 ( unsigned int rd, unsigned int ra, unsigned int rb)
{
//1100 00pp pppd dddd aaaa accc c00b bbbb
//010p pppp ssss dddd   "; %s{p} r%i{d}, r%i{s}"
    rd=adreg(rd);
    ra=adreg(ra);
    rb=adreg(rb);
    if(pass==0)
    {
        if((rd>15)||(ra>15)||(rb>15))
        {
            output[doff][0]=2;
            output[doff][1]=0xC000|(0x0D<<5)|(rd);
            output[doff][2]=0x0000|(ra<<11)|(0xE<<7)|(rb);
        }
        else
        {
            if(rd==ra)
            {
                output[doff][0]=1;
                output[doff][1]=0x4000|(0x0D<<8)|(rb<<4)|rd;
            }
            else
            if(rd==rb)
            {
                output[doff][0]=1;
                output[doff][1]=0x4000|(0x0D<<8)|(ra<<4)|rd;
            }
            else
            {
                output[doff][0]=2;
                output[doff][1]=0xC000|(0x0D<<5)|(rd);
                output[doff][2]=0x0000|(ra<<11)|(0xE<<7)|(rb);
            }
        }
    }
    show_output();
}
//-------------------------------------------------------------------
void nor3 ( unsigned int rd, unsigned int ra, unsigned int rb)
{
//1100 00pp pppd dddd aaaa accc c00b bbbb
//010p pppp ssss dddd   "; %s{p} r%i{d}, r%i{s}"
    rd=adreg(rd);
    ra=adreg(ra);
    rb=adreg(rb);
    if(pass==0)
    {
        if(ra==0)
        {
            if((rd>15)||(rb>15))
            {
                //1100 00pp pppd dddd  aaaa accc c00b bbbb
                //mvn rd = ~rb, ra not used.
                output[doff][0]=2;
                output[doff][1]=0xC000|(P_MVN<<5)|(rd);
                output[doff][2]=0x0000|(CC_UN<<7)|(rb);
            }
            else
            {
                //010p pppp ssss dddd   "; %s{p} r%i{d}, r%i{s}"
                output[doff][0]=1;
                output[doff][1]=0x4000|(P_MVN<<8)|(rb<<4)|(rd);
            }
        }
        else
        if(rb==0)
        {
            if((rd>15)||(ra>15))
            {
                //1100 00pp pppd dddd  aaaa accc c00b bbbb
                //mvn rd = ~rb, ra not used.
                output[doff][0]=2;
                output[doff][1]=0xC000|(P_MVN<<5)|(rd);
                output[doff][2]=0x0000|(CC_UN<<7)|(ra);
            }
            else
            {
                //010p pppp ssss dddd   "; %s{p} r%i{d}, r%i{s}"
                output[doff][0]=1;
                output[doff][1]=0x4000|(P_MVN<<8)|(ra<<4)|(rd);
            }
        }
        else
        {
            if(rd>15)
            {
                output[doff][0]=4;
                //1100 00pp pppd dddd  aaaa accc c00b bbbb
                output[doff][1]=0xC000|(P_OR<<5)|(rd);
                output[doff][2]=0x0000|(CC_UN<<7)|(ra<<11)|(rb);
                //1100 00pp pppd dddd  aaaa accc c00b bbbb
                output[doff][3]=0xC000|(P_MVN<<5)|(rd);
                output[doff][4]=0x0000|(CC_UN<<7)|(rd);
            }
            else
            {
                //1100 00pp pppd dddd  aaaa accc c00b bbbb
                output[doff][0]=3;
                output[doff][1]=0xC000|(P_OR<<5)|(rd);
                output[doff][2]=0x0000|(CC_UN<<7)|(ra<<11)|(rb);
                //010p pppp ssss dddd   "; %s{p} r%i{d}, r%i{s}"
                output[doff][3]=0x4000|(P_MVN<<8)|(rd<<4)|(rd);
            }
        }
    }
    show_output();
}
//-------------------------------------------------------------------
void mov ( unsigned int rd, unsigned int rs)
{
//1100 00pp pppd dddd aaaa accc c00b bbbb
//010p pppp ssss dddd   "; %s{p} r%i{d}, r%i{s}"
//#define movs(reg, regs) emit2(0xc000|(reg), 0xcf00|(regs))

    rd=adreg(rd);
    rs=adreg(rs);
    if(pass==0)
    {
        if((rd>15)||(rs>15))
        {
            output[doff][0]=2;
            output[doff][1]=0xC000|(0x00<<5)|(rd);
            output[doff][2]=0x0000|(rs<<11)|(0xE<<7)|(rs);
        }
        else
        {
            output[doff][0]=1;
            output[doff][1]=0x4000|(0x00<<8)|(rs<<4)|rd;
        }
    }
    show_output();
}//-------------------------------------------------------------------
//-------------------------------------------------------------------
void add3i ( unsigned int rd, unsigned int rs, unsigned int data )
{
//1110 11ss sssd dddd  uuuu uuuu uuuu uuuu  uuuu uuuu uuuu uuuu       "; add r%i{d}, r%i{s}, #0x%08x{u}"

    rd=adreg(rd);
    rs=adreg(rs);
    if(pass==0)
    {
        output[doff][0]=3;
        output[doff][1]=0xEC00|(rs<<5)|(rd);
        output[doff][2]=data&0xFFFF;
        output[doff][3]=(data>>16)&0xFFFF;
    }
    show_output();
}
//-------------------------------------------------------------------
void and3i ( unsigned int rd, unsigned int rs, unsigned int data )
{
//1110 10pp pppd dddd uuuu uuuu uuuu uuuu uuuu uuuu uuuu uuuu       "; %s{p} r%i{d}, r%i{s}, #0x%08x{u}"
//1011 00pp pppd dddd iiii iiii iiii iiii
//010p pppp ssss dddd
//1100 00pp pppd dddd  aaaa accc c00b bbbb
//1100 00pp pppd dddd  aaaa accc c1ii iiii                           "; %s{p}%s{c} r%i{d}, r%i{a}, #%i{i}"

    rd=adreg(rd);
    rs=adreg(rs);
    if(pass==0)
    {
        if(rs==rd)
        {
            output[doff][0]=3;
            //1110 10pp pppd dddd  uuuu uuuu uuuu uuuu  uuuu uuuu uuuu uuuu       "; %s{p} r%i{d}, r%i{s}, #0x%08x{u}"
            output[doff][1]=0xE800|(P_AND<<5)|(rd);
            output[doff][2]=data&0xFFFF;
            output[doff][3]=(data>>16)&0xFFFF;
        }
        else
        {
            if((rd>15)||(rs>15))
            {
                if((data&0xFFFF)==data) //this should always be the case with mips immediate
                {
                    if((data&0x3F)==data)
                    {
                        output[doff][0]=2;
                        output[doff][1]=0xC000|(P_AND<<5)|rd;
                        output[doff][2]=0x0040|(rs<<11)|(0xE<<7)|data;
                    }
                    else
                    {
                        output[doff][0]=4;
                        output[doff][1]=0xB000|(0x00<<5)|(rd);
                        output[doff][2]=data;
                        output[doff][3]=0xC000|(P_AND<<5)|rd;
                        output[doff][4]=0x0040|(rs<<11)|(0xE<<7)|0;
                    }
                }
                else
                {
                    printf("zz ori(r%u,r%u,0x%04X)\n",rd,rs,data);
                }
            }
            else
            {
                if((data&0x7FFF)==data) //this should always be the case with mips immediate
                {
                    if((data&0x3F)==data)
                    {
                        output[doff][0]=2;
                        output[doff][1]=0xC000|(P_AND<<5)|rd;
                        output[doff][2]=0x0040|(rs<<11)|(0xE<<7)|data;
                    }
                    else
                    {
                        output[doff][0]=3;
                        //1011 00pp pppd dddd iiii iiii iiii iiii
                        output[doff][1]=0xB000|(P_MOV<<5)|(rd);
                        output[doff][2]=data;
                        //010p pppp ssss dddd
                        output[doff][3]=0x4000|(P_AND<<8)|(rs<<4)|rd;
                    }
                }
                else
                {
                    printf("xx ori(r%u,r%u,0x%04X)\n",rd,rs,data);
                }
            }
        }
    }
    show_output();
}

//-------------------------------------------------------------------
void or3i ( unsigned int rd, unsigned int rs, unsigned int data )
{
//1110 10pp pppd dddd uuuu uuuu uuuu uuuu uuuu uuuu uuuu uuuu       "; %s{p} r%i{d}, r%i{s}, #0x%08x{u}"
//1011 00pp pppd dddd iiii iiii iiii iiii
//010p pppp ssss dddd
//1100 00pp pppd dddd  aaaa accc c00b bbbb
//1100 00pp pppd dddd  aaaa accc c1ii iiii                           "; %s{p}%s{c} r%i{d}, r%i{a}, #%i{i}"

    rd=adreg(rd);
    rs=adreg(rs);
    if(pass==0)
    {
        if(rs==rd)
        {
            output[doff][0]=3;
            //1110 10pp pppd dddd  uuuu uuuu uuuu uuuu  uuuu uuuu uuuu uuuu       "; %s{p} r%i{d}, r%i{s}, #0x%08x{u}"
            output[doff][1]=0xE800|(P_OR<<5)|(rd);
            output[doff][2]=data&0xFFFF;
            output[doff][3]=(data>>16)&0xFFFF;
        }
        else
        {
            if((rd>15)||(rs>15))
            {
                if((data&0x7FFF)==data) //this should always be the case with mips immediate
                {
                    if((data&0x1F)==data)
                    {
                        output[doff][0]=2;
                        output[doff][1]=0xC000|(P_OR<<5)|rd;
                        output[doff][2]=0x0040|(rs<<11)|(0xE<<7)|data;
                    }
                    else
                    {
                        output[doff][0]=4;
                        output[doff][1]=0xB000|(0x00<<5)|(rd);
                        output[doff][2]=data;
                        output[doff][3]=0xC000|(P_OR<<5)|rd;
                        output[doff][4]=0x0040|(rs<<11)|(0xE<<7)|0;
                    }
                }
                else
                {
                    printf("zz ori(r%u,r%u,0x%04X)\n",rd,rs,data);
                }
            }
            else
            {
                if((data&0x7FFF)==data) //this should always be the case with mips immediate
                {
                    if((data&0x1F)==data)
                    {
                        output[doff][0]=2;
                        output[doff][1]=0xC000|(P_OR<<5)|rd;
                        output[doff][2]=0x0040|(rs<<11)|(0xE<<7)|data;
                    }
                    else
                    {
                        output[doff][0]=3;
                        //1011 00pp pppd dddd iiii iiii iiii iiii
                        output[doff][1]=0xB000|(P_MOV<<5)|(rd);
                        output[doff][2]=data;
                        //010p pppp ssss dddd
                        output[doff][3]=0x4000|(P_OR<<8)|(rs<<4)|rd;
                    }
                }
                else
                {
                    printf("xx ori(r%u,r%u,0x%04X)\n",rd,rs,data);
                }
            }
        }
    }
    show_output();
}
//-------------------------------------------------------------------
void b ( unsigned int address )
{
    unsigned int ra,rb;
    //1001 cccc 0ooo oooo   oooo oooo oooo oooo    b<cc> $+o*2
    output[doff][0]=2;
    output[doff][1]=0x9E00;
    output[doff][2]=0x0000;
    if(pass)
    {
        if(address==addr)
        {
        }
        else
        if(address<addr)
        {
            rb=0;
            for(ra=(address>>2);ra<doff;ra++)
            {
                if(hit[ra]&&output[ra][0]==0)
                {
                    printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",ra<<2,addr);
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
            for(ra=doff;ra<(address>>2);ra++)
            {
                if(hit[ra]&&output[ra][0]==0)
                {
                    printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",ra<<2,addr);
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
void bl ( unsigned int address )
{
    unsigned int ra,rb;

    //1001 oooo 1ooo oooo   oooo oooo oooo oooo
    output[doff][0]=2;
    output[doff][1]=0x9080;
    output[doff][2]=0x0000;
    if(pass)
    {
        if(address==addr)
        {
            printf("// bl to self\n");
        }
        else
        if(address<addr)
        {
            rb=0;
            for(ra=(address>>2);ra<doff;ra++)
            {
                if(hit[ra]&&output[ra][0]==0)
                {
                    printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",ra<<2,addr);
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
        }
        else
        {
            rb=0;
            for(ra=doff;ra<(address>>2);ra++)
            {
                if(hit[ra]&&output[ra][0]==0)
                {
                    printf("[0x%08X] empty output, cannot resolve [0x%08X]\n",ra<<2,addr);
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
        }
    }
    show_output();
}
//-------------------------------------------------------------------
void movi ( unsigned int reg, unsigned int data )
{
//#define movi(reg, imm)      emit3(0xe800|(0x0<<5)|(reg), (imm)&0xffff, ((imm)>>16)&0xffff)
//1110 10pp pppd dddd  uuuu uuuu uuuu uuuu  uuuu uuuu uuuu uuuu
    reg=adreg(reg);
    if(pass==0)
    {
        output[doff][0]=3;
        output[doff][1]=0xE800|(0x00<<5)|reg;
        output[doff][2]=(data&0xFFFF);
        output[doff][3]=(data>>16)&0xFFFF;
    }
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
}//-------------------------------------------------------------------
void str ( unsigned int rd, unsigned int rs, unsigned int offset )
{
//0011 uuuu ssss dddd                                               "; st  r%i{d}, 0x%02x{u*4}(r%i{s})"
//1010 0010 ww1d dddd ssss suuu uuuu uuuu                           "; st%s{w} r%i{d}, 0x%x{u}(r%i{s})"
//1110 0110 ww1d dddd ssss sooo oooo oooo oooo oooo oooo oooo       "; st%s{w} r%i{d}, 0x%08x{o}(r%i{s})"
    rd=adreg(rd);
    rs=adreg(rs);
    if(offset==0)
    {
        if((rd<16)&&(rs<16))
        {
            //0000 1001 ssss dddd                                               "; st%s{w} r%i{d}, (r%i{s})"
            output[doff][0]=1;
            output[doff][1]=0x0900|(rs<<4)|(rd);
        }
        else
        {
            //1010 0010 ww1d dddd  ssss suuu uuuu uuuu                           "; st%s{w} r%i{d}, 0x%x{u}(r%i{s})"
            output[doff][0]=2;
            output[doff][1]=0xA220|(rd);
            output[doff][2]=rs<<11;
        }
    }
    else
    {
        if((rd<16)&&(rs<16)&&((offset&0x3C)==offset))
        {
            //0011 uuuu ssss dddd                                               "; st  r%i{d}, 0x%02x{u*4}(r%i{s})"
            output[doff][0]=1;
            output[doff][1]=0x3000|(offset<<(8-2))|(rs<<4)|rd;
        }
        else
        if((offset&0x7FF)==offset)
        {
            //1010 0010 ww1d dddd  ssss suuu uuuu uuuu                           "; st%s{w} r%i{d}, 0x%x{u}(r%i{s})"
            output[doff][0]=2;
            output[doff][1]=0xA220|rd;
            output[doff][2]=(rs<<11)|offset;
        }
        else
        {
        }
    }
    show_output();
}
//-------------------------------------------------------------------
void rts ( void )
{
//0000 0000 0101 1010                                               "; rts"     #otherwise known as 'b lr' (b r26)
//0000 0000 010d dddd                                               "; b r%i{d}"
    if(pass==0)
    {
        output[doff][0]=1;
        output[doff][1]=0x005A;
    }
    show_output();
}
//-------------------------------------------------------------------
void branchdest ( unsigned int addr )
{
    unsigned int doff;

    doff=addr>>2;
if(doff>=MAXDATA)
{
    fprintf(stderr,"branchdest too big 0x%08X\n",addr);
    return;
}

    if(branch[doff]) return;
    branch[doff]=1;
    //fprintf(stderr,"branchdest(0x%08X)\n",addr);
}
//-------------------------------------------------------------------
void addhit ( unsigned int addr )
{
    unsigned int doff;

    doff=addr>>2;
if(doff>=MAXDATA)
{
    fprintf(stderr,"branchdest too big 0x%08X\n",addr);
    return;
}
    if(hit[doff]) return;
    hit[doff]=1;
    newhit=1;
    //fprintf(stderr,"addhit(0x%08X)\n",addr);
}
//-------------------------------------------------------------------
int main ( int argc, char *argv[] )
{
    unsigned int datalen;
    unsigned int dest;
    unsigned int inst,inst2;
    unsigned int run;
    unsigned int ra,rb,rc;
    unsigned int op;
    unsigned int fun;
    unsigned int rs,rt,rd,shift;
    unsigned int imm;


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
    datalen=(datalen+3)>>2;
    fprintf(stderr,"datalen %u words\n",datalen);
    if(datalen==0)
    {
        fprintf(stderr,"empty file\n");
        return(1);
    }

    //for(ra=0;ra<10;ra++) printf("0x%06X : 0x%08X\n",ra,data[ra]);
    for(doff=0;doff<datalen;doff++)
    {
        ra =(data[doff]&0x000000FF)<<24;
        ra|=(data[doff]&0x0000FF00)<<8;
        ra|=(data[doff]&0x00FF0000)>>8;
        ra|=(data[doff]&0xFF000000)>>24;
        data[doff]=ra;
    }
    //for(ra=0;ra<10;ra++) printf("0x%06X : 0x%08X\n",ra,data[ra]);

    memset(hit,0,sizeof(hit));
    memset(branch,0,sizeof(branch));
    memset(output,0,sizeof(output));

    addhit(0);
    while(1)
    {
        newhit=0;
        for(doff=0;doff<datalen;doff++)
        {
            if(hit[doff]==0) continue;
            addr=doff<<2;
            inst=data[doff];
            op=(inst>>26)&0x3F;
            fun=inst&0x3F;
            run=1;
            switch(op)
            {
                case 0x02: //j
                {
                    dest=inst&0x03FFFFFF;
                    dest<<=2;
//printf("0x%08X 0x%08X 0x%08X\n",addr,inst,dest);
                    branchdest(dest);
                    addhit(dest);
                    run=0;
                    break;
                }
                case 0x03: //jal
                {
                    dest=inst&0x03FFFFFF;
                    dest<<=2;
//printf("0x%08X 0x%08X 0x%08X\n",addr,inst,dest);
                    branchdest(dest);
                    addhit(dest);
                    run=0;
                    addhit(addr+8);
                    break;
                }
                case 0x00:
                {
                    switch(fun)
                    {
                        case 0x08: //jr
                        {
                            run=0;
                            break;
                        }
                        case 0x09: //jalr
                        {
                            run=0;
                            addhit(addr+8);
                            break;
                        }
                    }
                    break;
                }
                case 0x04: //beq
                case 0x05: //bne
                case 0x06: //blez
                case 0x07: //bgtz
                {
                    dest=inst&0xFFFF;
                    if(dest&0x8000) dest|=0xFFFF0000;
                    dest<<=2;
                    dest+=addr+4;
//printf("0x%08X 0x%08X 0x%08X\n",addr,inst,dest);
                    branchdest(dest);
                    addhit(dest);
                    run=0;
                    addhit(addr+8);
                    break;
                }
            }
            if(run) addhit(addr+4);
        }
        if(newhit==0) break;
    }
    //swap jumps and fix branch shadow
    for(doff=0;doff<datalen;doff++)
    {
        if(hit[doff]==0) continue;
        newhit=0;
        inst=data[doff];
        op=(inst>>26)&0x3F;
        fun=inst&0x3F;
        switch(op)
        {
            case 0x02: newhit=1; break; //j
            case 0x03: newhit=1; break; //jal
            case 0x04: newhit=1; break; //beq
            case 0x05: newhit=1; break; //bne
            case 0x06: newhit=1; break; //blez
            case 0x07: newhit=1; break; //bgtz
            case 0x00:
            {
                switch(fun)
                {
                    case 0x08: newhit=1; break; //jr
                    case 0x09: newhit=1; break; //jalr
                }
                break;
            }
        }
        if(newhit)
        {
            data[doff]=data[doff+1];
            doff++;
            data[doff]=inst;
            hit[doff]=1;
        }
    }

// NEED TO OPTMIZE THE SLTU STUFF

//  c4: 0050102b    sltu    v0,v0,s0
//  c8: 1440fffb    bnez    v0,b8 <notmain+0x90>
//  cc: 3c047e20    lui a0,0x7e20

// replace with
// movi a0,0x7E20
// cmp (v0,s0);
// bltu b8


    for(pass=0;pass<2;pass++)
    {
        printf("// ----- pass %u -----\n",pass);

        if(pass)
        {
            //printf("fillb(0x200,0);\n");
            printf("//make r0 zero\n");
//# op rd, #u   (rd = rd op #u)
//011q qqqu uuuu dddd                                               "; %s{q} r%i{d}, #%i{u}"
            printf("emit1(0x%04X);\n",0x6000|(Q_MOV<<9)|(0<<4)|(0));
            dissadd=2;
        }
        for(doff=0;doff<datalen;doff++)
        {
            addr=doff<<2;
            inst=data[doff];
            if(hit[doff]==0) continue;
            if(pass) printf("\n");
            printf("//[0x%08X] 0x%08X ",addr,inst);
            op=(inst>>26)&0x3F;
            fun=inst&0x3F;
            printf("(%02X)",op);
            if(op==0) printf("(%02X)  ",fun);
            else        printf("      ");
            rt=(inst>>16)&0x1F;
            rs=(inst>>21)&0x1F;
            imm=(inst&0xFFFF);
            switch(op)
            {
                case 0x02: //j
                {
                    dest=inst&0x03FFFFFF;
                    dest<<=2;
                    printf("j 0x%08X\n",dest);
                    b(dest);
                    break;
                }
                case 0x03: //jal
                {
                    dest=inst&0x03FFFFFF;
                    dest<<=2;
                    printf("jal 0x%08X\n",dest);
                    bl(dest);
                    break;
                }
                case 0x04: //beq $s,$t,offset/label
                {
                    dest=inst&0xFFFF;
                    if(dest&0x8000) dest|=0xFFFF0000;
                    dest<<=2;
                    dest+=addr;
                    printf("beq r%u,r%u,0x%08X\n",rs,rt,dest);
                    cmp_beq(rs,rt,dest);
                    break;
                }
                case 0x05: //bne $s,$t,offset/label
                {
                    dest=inst&0xFFFF;
                    if(dest&0x8000) dest|=0xFFFF0000;
                    dest<<=2;
                    dest+=addr;
                    printf("bne r%u,r%u,0x%08X\n",rs,rt,dest);
                    cmp_bne(rs,rt,dest);
                    break;
                }
                case 0x06: //blez $s,$t,offset/label
                {
                    dest=inst&0xFFFF;
                    if(dest&0x8000) dest|=0xFFFF0000;
                    dest<<=2;
                    dest+=addr;
                    printf("blez r%u,r%u,0x%08X\n",rs,rt,dest);
                    break;
                }
                case 0x07: //bgtz $s,$t,offset/label
                {
                    dest=inst&0xFFFF;
                    if(dest&0x8000) dest|=0xFFFF0000;
                    dest<<=2;
                    dest+=addr;
                    printf("bgtz r%u,r%u,0x%08X\n",rs,rt,dest);
                    break;
                }

                case 0x09: //addiu
                {
                    if(imm&0x8000)
                    {
                        imm|=0xFFFF0000;
                        ra=(~imm)+1;
                        printf("addiu r%u,r%u,-0x%04X\n",rt,rs,ra);
                    }
                    else
                    {
                        printf("addiu r%u,r%u,0x%04X\n",rt,rs,imm);
                    }
                    pc2u(P_ADD,CC_UN,rt,rs,imm);
                    break;
                }
                case 0x0C: //and rt,rs,imm
                {
                    printf("andi r%u,r%u,0x%04X\n",rt,rs,imm);
                    pc2u(P_AND,CC_UN,rt,rs,imm);
                    break;
                }
                case 0x0D: //ori rt,rs,imm
                {
                    printf("ori r%u,r%u,0x%04X\n",rt,rs,imm);
                    pc2u(P_OR,CC_UN,rt,rs,imm);
                    break;
                }
                case 0x0F: //lui
                {
                    printf("lui r%u,0x%04X\n",rt,imm);
                    movi(rt,imm<<16);
                    break;
                }
                case 0x23: //lw
                {
                    if(imm==0)
                    {
                        printf("lw r%u,(r%u)\n",rt,rs);
                    }
                    else
                    {
                        printf("lw r%u,0x%04X(r%u)\n",rt,imm,rs);
                    }
                    ldr(rt,rs,imm);
                    break;
                }
                case 0x2B: //sw
                {
                    if(imm==0)
                    {
                        printf("sw r%u,(r%u)\n",rt,rs);
                    }
                    else
                    {
                        printf("sw r%u,0x%04X(r%u)\n",rt,imm,rs);
                    }
                    str(rt,rs,imm);
                    break;
                }
                default:
                {
                    if(op==0x00)
                    {
                        if(inst==0x00000000)
                        {
                            printf("nop\n");
                            nop();
                            break;
                        }
                        shift=(inst>>6)&0x1F;
                        rd=(inst>>11)&0x1F;
                        switch(fun)
                        {
                            case 0x08:
                            {
                                printf("jr r%u\n",rs);
                                if(rs==31)
                                {
                                    rts();
                                }
                                else
                                {
                                    printf("WARNING\n");
                                }
                                break;
                            }
                            case 0x09:
                            {
                                printf("jalr r%u\n",rs);
                                break;
                            }
                            case 0x21: //addu rd,rs,rt
                            {
                                if(rt==0)
                                {
                                    printf("mov r%u,r%u\n",rd,rs);
                                    mov(rd,rs);
                                }
                                else if(rs==0)
                                {
                                    printf("mov r%u,r%u\n",rd,rt);
                                    mov(rd,rt);
                                }
                                else
                                {
                                    printf("addu r%u,r%u,r%u\n",rd,rs,rt);
                                    pc3(P_ADD,CC_UN,rd,rs,rt);

                                }
                                break;
                            }
                            case 0x23: //subu rd,rs,rt
                            {
                                printf("subu r%u,r%u,r%u\n",rd,rs,rt);
                                pc3(P_SUB,CC_UN,rd,rs,rt);
                                break;
                            }
                            case 0x24: //and rd,rs,rt
                            {
                                printf("and r%u,r%u,r%u\n",rd,rs,rt);
                                pc3(P_AND,CC_UN,rd,rs,rt);
                                break;
                            }
                            case 0x25: //or rd,rs,rt
                            {
                                printf("or r%u,r%u,r%u\n",rd,rs,rt);
                                pc3(P_OR,CC_UN,rd,rs,rt);
                                break;
                            }
                            case 0x27: //nor rd,rs,rt
                            {
                                printf("nor r%u,r%u,r%u\n",rd,rs,rt);
                                nor3(rd,rs,rt);
                                break;
                            }
                            case 0x2B: //sltu rd,rs,rt
                            {
                                printf("sltu r%u,r%u,r%u\n",rd,rs,rt);
                                sltu(rd,rs,rt);
                                break;
                            }
                            default:
                            {
                                printf("UNKNOWN\n");
                                break;
                            }
                        }
                    }
                    else
                    {
                        printf("UNKNOWN\n");
                    }
                }
            }
            if(pass)
            {
                if(output[doff][0]==0)
                {
                    printf("[0x%08X] no output\n",addr);
                }
            }
        }
    }
    for(doff=0;doff<datalen;doff++)
    {
        if(hit[doff]==0) continue;
        addr=doff<<2;
        if(output[doff][0]==0)
        {
            printf("[0x%08X] no output\n",addr);
            fprintf(stderr,"[0x%08X] no output\n",addr);
        }
    }

    return(0);
}


//-------------------------------------------------------------------
void diss ( void )
{
    unsigned int x;
    unsigned int inst;
    unsigned int inst2;
    unsigned int inst3;
    unsigned int rd;
    unsigned int q;
    unsigned int p;
    unsigned int u;
    unsigned int w;
    unsigned int cc;
    unsigned int ra,rb;
    unsigned int rs,rt;
    unsigned int dest;

    for(x=1;x<=output[doff][0];x++,dissadd+=2)
    {
        inst=output[doff][x];
        printf("//<0x%06X> 0x%04X ",dissadd,inst);

        if(inst==0x0001)
        {
            printf("nop\n");
            continue;
        }

        //0000 0000 0101 1010              "; rts"     #otherwise known as 'b lr'
        //0000 0000 010d dddd              "; b r%i{d}"
        if(inst==0x005A)
        {
            rd=inst&0x1F;
            printf("rts/b r%u\n",rd);
            continue;
        }
        //0000 0000 010d dddd              "; b r%i{d}"
        if((inst&0xFFE0)==0x0040)
        {
            rd=inst&0x1F;
            printf("b r%u\n",rd);
            continue;
        }
        //0000 0010 1010 0000                                               "; push r6"
        //0000 0010 0010 0000                                               "; pop  r6"
        //0000 0011 1010 0000                                               "; pushl r6"
        //0000 0011 0010 0000                                               "; popl  r6"
        //0000 0010 1bb0 0000                                               "; push r%d{b*8}"
        if((inst&0xFF90)==0x0280)
        {
            switch((inst>>5)&3)
            {
                case 0: printf("push r0\n"); break;
                case 1: printf("push r6\n"); break;
                case 2: printf("push r16\n"); break;
                case 3: printf("push r24\n"); break;
            }
            continue;
        }
        //0000 0010 0bb0 0000                                               "; pop  r%d{b*8}"
        if((inst&0xFF90)==0x0200)
        {
            switch((inst>>5)&3)
            {
                case 0: printf("pop r0\n"); break;
                case 1: printf("pop r6\n"); break;
                case 2: printf("pop r16\n"); break;
                case 3: printf("pop r24\n"); break;
            }
            continue;
        }
        //0000 0011 1bb0 0000                                               "; pushl r%d{b*8}"
        if((inst&0xFF90)==0x0380)
        {
            switch((inst>>5)&3)
            {
                case 0: printf("pushl r0,lr\n"); break;
                case 1: printf("pushl r6,lr\n"); break;
                case 2: printf("pushl r16,lr\n"); break;
                case 3: printf("pushl r24,lr\n"); break;
            }
            continue;
        }
        //0000 0011 0bb0 0000                                               "; popl  r%d{b*8}"
        if((inst&0xFF90)==0x0300)
        {
            switch((inst>>5)&3)
            {
                case 0: printf("popl r0,pc\n"); break;
                case 1: printf("popl r6,pc\n"); break;
                case 2: printf("popl r16,pc\n"); break;
                case 3: printf("popl r24,pc\n"); break;
            }
            continue;
        }





        //0000 1ww0 ssss dddd      "; ld%s{w} r%i{d}, (r%i{s})"
        if((inst&0xF900)==0x0800)
        {
            w=(inst>>9)&0x3;
            rs=(inst>>4)&0xF;
            rd=inst&0xF;
            switch(w)
            {
                case 0: printf("ldr r%u,(r%u)\n",rd,rs); break;
                case 1: printf("ldrh r%u,(r%u)\n",rd,rs); break;
                case 2: printf("ldrb r%u,(r%u)\n",rd,rs); break;
                case 3: printf("ldUNKNOWN r%u,(r%u)\n",rd,rs); break;
            }
            continue;
        }
        //0000 1ww1 ssss dddd      "; st%s{w} r%i{d}, (r%i{s})"
        if((inst&0xF900)==0x0900)
        {
            w=(inst>>9)&0x3;
            rs=(inst>>4)&0xF;
            rd=inst&0xF;
            switch(w)
            {
                case 0: printf("str r%u,(r%u)\n",rd,rs); break;
                case 1: printf("strh r%u,(r%u)\n",rd,rs); break;
                case 2: printf("strb r%u,(r%u)\n",rd,rs); break;
                case 3: printf("stUNKNOWN r%u,(r%u)\n",rd,rs); break;
            }
            continue;
        }

        //# op rd, ra   (rd = rd op ra)
        //010p pppp ssss dddd                 "; %s{p} r%i{d}, r%i{s}"
        if((inst&0xE000)==0x4000)
        {
            p=(inst>>8)&0x1F;
            rs=(inst>>4)&0xF;
            rd=inst&0xF;
            switch(p)
            {
                case P_MOV: printf("mov r%u,r%u\n",rd,rs); break;
                case P_CMN: printf("cmn r%u,r%u\n",rd,rs); break;
                case P_ADD: printf("add r%u,r%u\n",rd,rs); break;
                case P_BIC: printf("bic r%u,r%u\n",rd,rs); break;
                case P_MUL: printf("mul r%u,r%u\n",rd,rs); break;
                case P_EOR: printf("eor r%u,r%u\n",rd,rs); break;
                case P_SUB: printf("sub r%u,r%u\n",rd,rs); break;
                case P_AND: printf("and r%u,r%u\n",rd,rs); break;
                case P_MVN: printf("mvn r%u,r%u\n",rd,rs); break;
                case P_ROR: printf("ror r%u,r%u\n",rd,rs); break;
                case P_CMP: printf("cmp r%u,r%u\n",rd,rs); break;
                case P_RSB: printf("rsb r%u,r%u\n",rd,rs); break;

                case P_OR: printf("or r%u,r%u\n",rd,rs); break;



                default: printf("not done yet\n"); break;
            }
            continue;
        }

        //1001 cccc 0ooo oooo  oooo oooo oooo oooo            "; b%s{c} 0x%08x{$+o*2}"
        if((inst&0xF080)==0x9000)
        {
            inst2=output[doff][x+1];
            printf("0x%04X ",inst2);
            cc=(inst>>8)&0xF;
            dest=inst&0x7F;
            if(dest&0x40) dest|=(~0)<<7;
            dest<<=16;
            dest|=inst2;
            dest<<=1;
            dest+=dissadd;
            printf("b%s 0x%08X\n",cc_name[cc],dest);
            x++; dissadd+=2;
            continue;
        }
        //1001 oooo 1ooo oooo  oooo oooo oooo oooo      "; bl  0x%08x{$+o*2}"
        if((inst&0xF080)==0x9080)
        {
            inst2=output[doff][x+1];
            printf("0x%04X ",inst2);
            dest=inst&0x7F;
            if(dest&0x40) dest|=(~0)<<7;
            dest<<=16;
            dest|=inst2;
            dest<<=1;
            dest+=dissadd;
            printf("bl 0x%08X\n",dest);
            x++; dissadd+=2;
            continue;
        }
        //validated
        //# op rd, #u   (rd = rd op #u)
        //011q qqqu uuuu dddd                  "; %s{q} r%i{d}, #%i{u}"
        if((inst&0xE000)==0x6000)
        {
            q=(inst>>9)&0xF;
            u=(inst>>4)&0x1F;
            rd=inst&0xF;
            switch(q)
            {
                case Q_MOV: printf("mov r%u,#0x%X\n",rd,u); break;
                case Q_ADD: printf("add r%u,#0x%X\n",rd,u); break;
                case Q_MUL: printf("mul r%u,#0x%X\n",rd,u); break;
                case Q_SUB: printf("sub r%u,#0x%X\n",rd,u); break;
                case Q_MVN: printf("mvn r%u,#0x%X\n",rd,u); break;
                case Q_CMP: printf("cmp r%u,#0x%X\n",rd,u); break;
                default: printf("unknown\n"); break;
            }
            continue;
        }

        //1010 0010 ww0d dddd   ssss suuu uuuu uuuu                           "; ld%s{w} r%i{d}, 0x%x{u}(r%i{s})"
        if((inst&0xFF20)==0xA200)
        {
            inst2=output[doff][x+1];
            printf("0x%04X ",inst2);
            w=(inst>>6)&3;
            rd=inst&0x1F;
            rs=(inst2>>11)&0x1F;
            u=inst2&0x7FF;
            switch(w)
            {
                case 0: printf("ldr r%u,(r%u+0x%X)\n",rd,rs,u); break;
                case 1: printf("ldrb r%u,(r%u+0x%X)\n",rd,rs,u); break;
                case 2: printf("ldrh r%u,(r%u+0x%X)\n",rd,rs,u); break;
                case 3: printf("ldrUNKNOWN r%u,(r%u+0x%X)\n",rd,rs,u); break;
            }
            x++; dissadd+=2;
            continue;
        }
        //1010 0010 ww1d dddd ssss suuu uuuu uuuu                         "; st%s{w} r%i{d}, 0x%x{u}(r%i{s})"
        if((inst&0xFF20)==0xA220)
        {
            inst2=output[doff][x+1];
            printf("0x%04X ",inst2);
            w=(inst>>6)&3;
            rd=inst&0x1F;
            rs=(inst2>>11)&0x1F;
            u=inst2&0x7FF;
            switch(w)
            {
                case 0: printf("str r%u,(r%u+0x%X)\n",rd,rs,u); break;
                case 1: printf("strb r%u,(r%u+0x%X)\n",rd,rs,u); break;
                case 2: printf("strh r%u,(r%u+0x%X)\n",rd,rs,u); break;
                case 3: printf("strUNKNOWN r%u,(r%u+0x%X)\n",rd,rs,u); break;
            }
            x++; dissadd+=2;
            continue;
        }
        //1011 00pp pppd dddd   iiii iiii iiii iiii         "; %s{p} r%i{d}, #0x%04x{i}"
        if((inst&0xFC00)==0xB000)
        {
            inst2=output[doff][x+1];
            printf("0x%04X ",inst2);
            p=(inst>>5)&0x1F;
            rd=inst&0x1F;
            u=inst2;
            printf("%s r%u,0x%X\n",p_name[p],rd,u);
            x++; dissadd+=2;
            continue;
        }
        //1100 00pp pppd dddd   aaaa accc c00b bbbb   "; %s{p}%s{c} r%i{d}, r%i{a}, r%i{b}"
        if((inst&0xFC00)==0xC000)
        {
            inst2=output[doff][x+1];
            if((inst2&0x0060)==0x0000)
            {
                printf("0x%04X ",inst2);
                p=(inst>>5)&0x1F;
                rd=inst&0x1F;
                ra=(inst2>>11)&0x1F;
                cc=(inst2>>7)&0xF;
                rb=inst2&0x1F;
                printf("%s%s r%u,r%u,r%u\n",p_name[p],cc_name[cc],rd,ra,rb);
                x++; dissadd+=2;
                continue;
            }
        }
        //1100 00pp pppd dddd   aaaa accc c1ii iiii   "; %s{p}%s{c} r%i{d}, r%i{a}, #%i{i}"
        if((inst&0xFC00)==0xC000)
        {
            inst2=output[doff][x+1];
            if((inst2&0x0040)==0x0040)
            {
                printf("0x%04X ",inst2);
                p=(inst>>5)&0x1F;
                rd=inst&0x1F;
                ra=(inst2>>11)&0x1F;
                cc=(inst2>>7)&0xF;
                u=inst2&0x3F;
                printf("%s%s r%u,r%u,0x%X\n",p_name[p],cc_name[cc],rd,ra,u);
                x++; dissadd+=2;
                continue;
            }
        }










        //1110 10pp pppd dddd   uuuu uuuu uuuu uuuu   uuuu uuuu uuuu uuuu       "; %s{p} r%i{d}, r%i{s}, #0x%08x{u}"
        if((inst&0xFC00)==0xE800)
        {
            inst2=output[doff][x+1];
            printf("0x%04X ",inst2);
            inst3=output[doff][x+2];
            printf("0x%04X ",inst3);
            p=(inst>>5)&0x1F;
            rd=inst&0x1F;
            u=inst2|(inst3<<16);
            switch(p)
            {
                case P_MOV: printf("mov r%u,0x%08X\n",rd,u); break;
                case P_CMN: printf("cmn r%u,0x%08X\n",rd,u); break;
                case P_ADD: printf("add r%u,0x%08X\n",rd,u); break;

                case P_CMP: printf("cmp r%u,0x%08X\n",rd,u); break;

                case P_OR: printf("or r%u,0x%08X\n",rd,u); break;

                default: printf("unknown\n"); break;
            }
            x++; dissadd+=2;
            x++; dissadd+=2;
            continue;
        }
        //validated
        //1110 11ss sssd dddd   uuuu uuuu uuuu uuuu   uuuu uuuu uuuu uuuu       "; add r%i{d}, r%i{s}, #0x%08x{u}"
        if((inst&0xFC00)==0xEC00)
        {
            inst2=output[doff][x+1];
            printf("0x%04X ",inst2);
            inst3=output[doff][x+2];
            printf("0x%04X ",inst3);
            rs=(inst>>5)&0x1F;
            rd=inst&0x1F;
            u=inst2|(inst3<<16);
            printf("add r%u,r%u,0x%08X\n",rd,rs,u);
            x++; dissadd+=2;
            x++; dissadd+=2;
            continue;
        }




        printf("UNKNOWN\n");
    }
}

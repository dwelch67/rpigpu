

//-------------------------------------------------------------------
void diss ( void )
{
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
    unsigned int rs;
    int sa;

        inst=code[0];
        fprintf(fpout,"//0x%04X ",inst);

        if(inst==0x0001)
        {
            fprintf(fpout,"nop\n");
            return;
        }

        //0000 0000 0101 1010              "; rts"     #otherwise known as 'b lr'
        //0000 0000 010d dddd              "; b r%i{d}"
        if(inst==0x005A)
        {
            rd=inst&0x1F;
            fprintf(fpout,"rts/b r%u\n",rd);
            return;
        }
        //0000 0000 010d dddd              "; b r%i{d}"
        if((inst&0xFFE0)==0x0040)
        {
            rd=inst&0x1F;
            fprintf(fpout,"b r%u\n",rd);
            return;
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
                case 0: fprintf(fpout,"push r0\n"); break;
                case 1: fprintf(fpout,"push r6\n"); break;
                case 2: fprintf(fpout,"push r16\n"); break;
                case 3: fprintf(fpout,"push r24\n"); break;
            }
            return;
        }
        //0000 0010 0bb0 0000                                               "; pop  r%d{b*8}"
        if((inst&0xFF90)==0x0200)
        {
            switch((inst>>5)&3)
            {
                case 0: fprintf(fpout,"pop r0\n"); break;
                case 1: fprintf(fpout,"pop r6\n"); break;
                case 2: fprintf(fpout,"pop r16\n"); break;
                case 3: fprintf(fpout,"pop r24\n"); break;
            }
            return;
        }
        //0000 0011 1bb0 0000                                               "; pushl r%d{b*8}"
        if((inst&0xFF90)==0x0380)
        {
            switch((inst>>5)&3)
            {
                case 0: fprintf(fpout,"pushl r0,lr\n"); break;
                case 1: fprintf(fpout,"pushl r6,lr\n"); break;
                case 2: fprintf(fpout,"pushl r16,lr\n"); break;
                case 3: fprintf(fpout,"pushl r24,lr\n"); break;
            }
            return;
        }
        //0000 0011 0bb0 0000                                               "; popl  r%d{b*8}"
        if((inst&0xFF90)==0x0300)
        {
            switch((inst>>5)&3)
            {
                case 0: fprintf(fpout,"popl r0,pc\n"); break;
                case 1: fprintf(fpout,"popl r6,pc\n"); break;
                case 2: fprintf(fpout,"popl r16,pc\n"); break;
                case 3: fprintf(fpout,"popl r24,pc\n"); break;
            }
            return;
        }





        //0000 1ww0 ssss dddd      "; ld%s{w} r%i{d}, (r%i{s})"
        if((inst&0xF900)==0x0800)
        {
            w=(inst>>9)&0x3;
            rs=(inst>>4)&0xF;
            rd=inst&0xF;
            switch(w)
            {
                case 0: fprintf(fpout,"ldr r%u,(r%u)\n",rd,rs); break;
                case 1: fprintf(fpout,"ldrh r%u,(r%u)\n",rd,rs); break;
                case 2: fprintf(fpout,"ldrb r%u,(r%u)\n",rd,rs); break;
                case 3: fprintf(fpout,"ldUNKNOWN r%u,(r%u)\n",rd,rs); break;
            }
            return;
        }
        //0000 1ww1 ssss dddd      "; st%s{w} r%i{d}, (r%i{s})"
        if((inst&0xF900)==0x0900)
        {
            w=(inst>>9)&0x3;
            rs=(inst>>4)&0xF;
            rd=inst&0xF;
            switch(w)
            {
                case 0: fprintf(fpout,"str r%u,(r%u)\n",rd,rs); break;
                case 1: fprintf(fpout,"strh r%u,(r%u)\n",rd,rs); break;
                case 2: fprintf(fpout,"strb r%u,(r%u)\n",rd,rs); break;
                case 3: fprintf(fpout,"stUNKNOWN r%u,(r%u)\n",rd,rs); break;
            }
            return;
        }



        //0001 1ccc cooo oooo                                               "; b%s{c} 0x%08x{$+o*2}"
        if((inst&0xF800)==0x1800)
        {
            cc=(inst>>7)&0xF;
            sa=inst&0x7F;
            if(sa&0x40) sa|=(~0)<<7;
            fprintf(fpout,"b%s %d\n",cc_name[cc],sa);
            return;
        }



        //0010 uuuu ssss dddd                                               "; ld  r%i{d}, 0x%02x{u*4}(r%i{s})"
        if((inst&0xF000)==0x2000)
        {
            u=(inst>>8)&0xF;
            u<<=2;
            rs=(inst>>4)&0xF;
            rd=(inst>>0)&0xF;
            fprintf(fpout,"ldr r%u,0x%X(r%u)\n",rd,u,rs);
            return;
        }
        //0011 uuuu ssss dddd                                               "; st  r%i{d}, 0x%02x{u*4}(r%i{s})"
        if((inst&0xF000)==0x3000)
        {
            u=(inst>>8)&0xF;
            u<<=2;
            rs=(inst>>4)&0xF;
            rd=(inst>>0)&0xF;
            fprintf(fpout,"str r%u,0x%X(r%u)\n",rd,u,rs);
            return;
        }


        //# op rd, ra   (rd = rd op ra)
        //010p pppp ssss dddd                 "; %s{p} r%i{d}, r%i{s}"
        if((inst&0xE000)==0x4000)
        {
            p=(inst>>8)&0x1F;
            rs=(inst>>4)&0xF;
            rd=inst&0xF;
            fprintf(fpout,"%s r%u,r%u\n",p_name[p],rd,rs);
            return;
        }

        //validated
        //# op rd, #u   (rd = rd op #u)
        //011q qqqu uuuu dddd                  "; %s{q} r%i{d}, #%i{u}"
        if((inst&0xE000)==0x6000)
        {
            q=(inst>>9)&0xF;
            u=(inst>>4)&0x1F;
            rd=inst&0xF;
            fprintf(fpout,"%s r%u,#0x%X\n",q_name[q],rd,u);
            return;
        }

        //1010 0010 ww0d dddd   ssss suuu uuuu uuuu                           "; ld%s{w} r%i{d}, 0x%x{u}(r%i{s})"
        if((inst&0xFF20)==0xA200)
        {
            inst2=code[1];
            fprintf(fpout,"0x%04X ",inst2);
            w=(inst>>6)&3;
            rd=inst&0x1F;
            rs=(inst2>>11)&0x1F;
            u=inst2&0x7FF;
            switch(w)
            {
                case 0: fprintf(fpout,"ldr r%u,(r%u+0x%X)\n",rd,rs,u); break;
                case 1: fprintf(fpout,"ldrb r%u,(r%u+0x%X)\n",rd,rs,u); break;
                case 2: fprintf(fpout,"ldrh r%u,(r%u+0x%X)\n",rd,rs,u); break;
                case 3: fprintf(fpout,"ldrUNKNOWN r%u,(r%u+0x%X)\n",rd,rs,u); break;
            }

            return;
        }
        //1010 0010 ww1d dddd   ssss suuu uuuu uuuu                         "; st%s{w} r%i{d}, 0x%x{u}(r%i{s})"
        if((inst&0xFF20)==0xA220)
        {
            inst2=code[1];
            fprintf(fpout,"0x%04X ",inst2);
            w=(inst>>6)&3;
            rd=inst&0x1F;
            rs=(inst2>>11)&0x1F;
            u=inst2&0x7FF;
            switch(w)
            {
                case 0: fprintf(fpout,"str r%u,(r%u+0x%X)\n",rd,rs,u); break;
                case 1: fprintf(fpout,"strh r%u,(r%u+0x%X)\n",rd,rs,u); break;
                case 2: fprintf(fpout,"strb r%u,(r%u+0x%X)\n",rd,rs,u); break;
                case 3: fprintf(fpout,"strUNKNOWN r%u,(r%u+0x%X)\n",rd,rs,u); break;
            }

            return;
        }
        //1011 00pp pppd dddd   iiii iiii iiii iiii         "; %s{p} r%i{d}, #0x%04x{i}"
        if((inst&0xFC00)==0xB000)
        {
            inst2=code[1];
            fprintf(fpout,"0x%04X ",inst2);
            p=(inst>>5)&0x1F;
            rd=inst&0x1F;
            u=inst2;
            if(u&0x8000)
            {
                fprintf(fpout,"SIGEXT ");
                u|=0xFFFF0000;
            }
            fprintf(fpout,"%s r%u,0x%X\n",p_name[p],rd,u);

            return;
        }
        //1100 00pp pppd dddd   aaaa accc c00b bbbb   "; %s{p}%s{c} r%i{d}, r%i{a}, r%i{b}"
        if((inst&0xFC00)==0xC000)
        {
            inst2=code[1];
            if((inst2&0x0060)==0x0000)
            {
                fprintf(fpout,"0x%04X ",inst2);
                p=(inst>>5)&0x1F;
                rd=inst&0x1F;
                ra=(inst2>>11)&0x1F;
                cc=(inst2>>7)&0xF;
                rb=inst2&0x1F;
                fprintf(fpout,"%s%s r%u,r%u,r%u\n",p_name[p],cc_name[cc],rd,ra,rb);

                return;
            }
        }
        //1100 00pp pppd dddd   aaaa accc c1ii iiii   "; %s{p}%s{c} r%i{d}, r%i{a}, #%i{i}"
        if((inst&0xFC00)==0xC000)
        {
            inst2=code[1];
            if((inst2&0x0040)==0x0040)
            {
                fprintf(fpout,"0x%04X ",inst2);
                p=(inst>>5)&0x1F;
                rd=inst&0x1F;
                ra=(inst2>>11)&0x1F;
                cc=(inst2>>7)&0xF;
                u=inst2&0x3F;
                if(u&0x20)
                {
                    fprintf(fpout,"SIGEXT ");
                    u|=(~0)<<6;
                }
                fprintf(fpout,"%s%s r%u,r%u,0x%X\n",p_name[p],cc_name[cc],rd,ra,u);

                return;
            }
        }










        //1110 10pp pppd dddd   uuuu uuuu uuuu uuuu   uuuu uuuu uuuu uuuu       "; %s{p} r%i{d}, r%i{s}, #0x%08x{u}"
        if((inst&0xFC00)==0xE800)
        {
            inst2=code[1];
            fprintf(fpout,"0x%04X ",inst2);
            inst3=code[2];
            fprintf(fpout,"0x%04X ",inst3);
            p=(inst>>5)&0x1F;
            rd=inst&0x1F;
            u=inst2|(inst3<<16);
            fprintf(fpout,"%s r%u,0x%08X\n",p_name[p],rd,u);
            return;
        }
        //validated
        //1110 11ss sssd dddd   uuuu uuuu uuuu uuuu   uuuu uuuu uuuu uuuu       "; add r%i{d}, r%i{s}, #0x%08x{u}"
        if((inst&0xFC00)==0xEC00)
        {
            inst2=code[1];
            fprintf(fpout,"0x%04X ",inst2);
            inst3=code[2];
            fprintf(fpout,"0x%04X ",inst3);
            rs=(inst>>5)&0x1F;
            rd=inst&0x1F;
            u=inst2|(inst3<<16);
            fprintf(fpout,"add r%u,r%u,0x%08X\n",rd,rs,u);


            return;
        }




        fprintf(fpout,"UNKNOWN\n");

}

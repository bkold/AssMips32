#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iomanip>
#include <sstream>
#include <cctype>

//issues at the moment

//decimalToBinary may not output the right length needed
//decimalToBinary may not do twos compliment
//creat custom parsers for the needed instructions
//creat the pseudocode instructions like la


using namespace std;

ifstream inFile;
ofstream outFile;
static string memStore[1028];               //var type and name
static string memValues[1028];              //val value
static string instructionStore[1028];       //full instruction line
static int instructionMemAdd[1028];         //address in memory for an instruction
static string instructionHexStore[1028];    //full instruction line(hex)
static int instructionHexMemAdd[1028];      //address in memory for an instruction(hex)
static string branchStore[256];             //branch label
static int brancMemAdd[256];                //branch mem address
static int branchLocation=0;                //pointer for the branch array
static int memLocation=0;                   //pointer for the mem arrays
static int instructionLocation=0;           //pointer for the instruction arrays
static int instructionHexLocation=0;        //pointer for the instruction arrays
static int textLength=20;                   //length of
static const int offsetVal=268500992;       //mem address for the data section
static const int instructionVal=4194304;    //mem address of the text section

string decimalToBinary(int decimal) {
  string binary="";
  int isNeg=0;
  int t=0;
  if(decimal<0){
      isNeg=1;
      decimal=decimal-(2*decimal);
  }
  if(decimal==0){
      binary="0000000000000000";
      return binary;
  }
  while(decimal)  {
      t=1;
      binary.insert(0, 1, (decimal & 1) + '0');
      decimal >>= 1;
  }

  if(isNeg==1){
      int i=0;
      while(i<binary.length()){
          if(binary[i]=='0'){
            binary[i]='1';
          }else{
            binary[i]='0';
          }
        ++i;
      }
      //add 1
      int x=binary.find_last_of('0');
      while(x<binary.length()){
        if(binary[x]=='0'){
            binary[x]='1';
          }else{
            binary[x]='0';
          }
          ++x;
      }
      int length=binary.length();
      while(length<16){
        binary="1"+binary;
        length=binary.length();
      }

  }else{
        int length=binary.length();
        if(length<=16){
            while(length<16){
                binary="0"+binary;
                length=binary.length();
            }
        }else{
            while(length<26){
                binary="0"+binary;
                length=binary.length();
            }
        }
  }


  return binary;
}

//convert the binLine to hex and print it to the file
void binLineToHex(string binLine){

    string hexLine="";
    int count=0;
    for(int i=0; i<8; ++i){
        string temp="";
        for(int j=count; j<count+4; ++j){
            temp+=binLine[j];
        }
        int num=atoi(temp.c_str());
        switch(num){
            case 0: hexLine=hexLine+"0";
            break;
            case 1: hexLine=hexLine+"1";
            break;
            case 10: hexLine=hexLine+"2";
            break;
            case 11: hexLine=hexLine+"3";
            break;
            case 100: hexLine=hexLine+"4";
            break;
            case 101: hexLine=hexLine+"5";
            break;
            case 110: hexLine=hexLine+"6";
            break;
            case 111: hexLine=hexLine+"7";
            break;
            case 1000: hexLine=hexLine+"8";
            break;
            case 1001: hexLine=hexLine+"9";
            break;
            case 1010: hexLine=hexLine+"a";
            break;
            case 1011: hexLine=hexLine+"b";
            break;
            case 1100: hexLine=hexLine+"c";
            break;
            case 1101: hexLine=hexLine+"d";
            break;
            case 1110: hexLine=hexLine+"e";
            break;
            case 1111: hexLine=hexLine+"f";
            break;
        }
        count+=4;
    }
    instructionHexStore[instructionHexLocation]=hexLine;
    instructionHexLocation++;
}

//prints the structure of the "text", first col is the postion, second is the line, third is the memory address pointer
void pText(){
    cout<<"------text-------\n";
    int j=0;
    while(j<instructionLocation){
        cout<<j<<"\t"<<instructionStore[j];
        cout<<'\t'<<'\t'<<'\t';
        cout<<hex<<instructionMemAdd[j];
        ++j;
        cout<<"\n";
    }
    cout<<"------branches-------\n";
    j=0;
    while(j<branchLocation){
        cout<<j<<"\t"<<branchStore[j];
        cout<<'\t'<<'\t';
        cout<<brancMemAdd[j];
        ++j;
        cout<<"\n";
    }


}

//prints the structure of the "memory", first col is the postion, second is the pointer, third is the value in dec/latin
void pMemory(){
    int j=0;
    while(j<memLocation){
        cout<<j<<"\t"<<memStore[j];
        cout<<'\t';
        cout<<memValues[j];
        ++j;
        cout<<"\n";
    }
}

//returns the binary string of the register number
string getRegNum(string reg){
    string ret="";
    if(reg.compare("$0")==0 || reg.compare("$zero")==0){
        ret="00000";
    }else if(reg.compare("$1")==0 || reg.compare("$at")==0){
        ret="00001";
    }else if(reg.compare("$2")==0 || reg.compare("$v0")==0){
        ret="00010";
    }else if(reg.compare("$3")==0 || reg.compare("$v1")==0){
        ret="00011";
    }else if(reg.compare("$4")==0 || reg.compare("$a0")==0){
        ret="00100";
    }else if(reg.compare("$5")==0 || reg.compare("$a1")==0){
        ret="00101";
    }else if(reg.compare("$6")==0 || reg.compare("$a2")==0){
        ret="00110";
    }else if(reg.compare("$7")==0 || reg.compare("$a3")==0){
        ret="00111";
    }else if(reg.compare("$8")==0 || reg.compare("$t0")==0){
        ret="01000";
    }else if(reg.compare("$9")==0 || reg.compare("$t1")==0){
        ret="01001";
    }else if(reg.compare("$10")==0 || reg.compare("$t2")==0){
        ret="01010";
    }else if(reg.compare("$11")==0 || reg.compare("$t3")==0){
        ret="01011";
    }else if(reg.compare("$12")==0 || reg.compare("$t4")==0){
        ret="01100";
    }else if(reg.compare("$13")==0 || reg.compare("$t5")==0){
        ret="01101";
    }else if(reg.compare("$14")==0 || reg.compare("$t6")==0){
        ret="01110";
    }else if(reg.compare("$15")==0 || reg.compare("$t7")==0){
        ret="01111";
    }else if(reg.compare("$16")==0 || reg.compare("$s0")==0){
        ret="10000";
    }else if(reg.compare("$17")==0 || reg.compare("$s1")==0){
        ret="10001";
    }else if(reg.compare("$18")==0 || reg.compare("$s2")==0){
        ret="10010";
    }else if(reg.compare("$19")==0 || reg.compare("$s3")==0){
        ret="10011";
    }else if(reg.compare("$20")==0 || reg.compare("$s4")==0){
        ret="10100";
    }else if(reg.compare("$21")==0 || reg.compare("$s5")==0){
        ret="10101";
    }else if(reg.compare("$22")==0 || reg.compare("$s6")==0){
        ret="10110";
    }else if(reg.compare("$23")==0 || reg.compare("$s7")==0){
        ret="10111";
    }else if(reg.compare("$24")==0 || reg.compare("$t8")==0){
        ret="11000";
    }else if(reg.compare("$25")==0 || reg.compare("$t9")==0){
        ret="11001";
    }else if(reg.compare("$26")==0 || reg.compare("$k0")==0){
        ret="11010";
    }else if(reg.compare("$27")==0 || reg.compare("$k1")==0){
        ret="11011";
    }else if(reg.compare("$28")==0 || reg.compare("$gp")==0){
        ret="11100";
    }else if(reg.compare("$29")==0 || reg.compare("$sp")==0){
        ret="11101";
    }else if(reg.compare("$30")==0 || reg.compare("$s8")==0 || reg.compare("$fp")==0){
        ret="11110";
    }else if(reg.compare("$31")==0 || reg.compare("$ra")==0){
        ret="11111";
    }else{
        //error not a useable register
    }
    return ret;
}

//parse the line for the desired values
//for r format
string rFormat(string line){
        string rd=line.substr(0, line.find(','));
        line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
        string rs=line.substr(0, line.find(','));
        line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
        string rt=line.substr(0, line.find(' ')-1);
        string rdb=getRegNum(rd);
        string rsb=getRegNum(rs);
        string rtb=getRegNum(rt);
        string temp="";
        temp=rsb+rtb+rdb;
        return temp;
}

//for i format
string iFormat(string line){
        string rs=line.substr(0, line.find(','));
        line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
        string rt=line.substr(0, line.find(','));
        line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
        string rtb=getRegNum(rt);
        string rsb=getRegNum(rs);
        string imm=line.substr(0, line.find(' '));
        int immed=atoi(imm.c_str());
        string immb=decimalToBinary(immed);

        string temp=rsb+rtb+immb;
        return temp;


}

//for j format
string jFormat(string line){
        string imm=line.substr(0, line.find(' '));
        if(isdigit(imm[0])){//used integer
            int immed=atoi(imm.c_str());
            string immb=decimalToBinary(immed);
            int length=immb.length();
            if(immb[0]=='0'){
                while(length<26){
                    immb="0"+immb;
                    length=immb.length();
                }
            }
            if(immb[0]=='1'){
                while(length<26){
                    immb="1"+immb;
                    length=immb.length();
                }
            }
            return immb;
        }else{//used branch address
            int j=0;
            int mem;
            while(j<branchLocation){
                string s=branchStore[j];
                if(s.length()>0){
                    s=s.substr(0, s.length()-1);
                    if(s.compare(imm)==0){
                        mem=brancMemAdd[j];
                    }
                }
                j++;
            }

            string immb=decimalToBinary(mem);

            return immb;

        }




}

//for lw and sw and the like
string regFormat(string line){
    string rt=line.substr(0, line.find(','));
    line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
    string offset=line.substr(0, line.find('('));
    line=line.substr(line.find('(')+1, line.size()-line.find('('));
    string base=line.substr(0, line.find(')'));

    int off=atoi(offset.c_str());
    string baseb=getRegNum(base);
    string rtb=getRegNum(rt);
    string offsetb=decimalToBinary(off);

    string temp="";
    //temp=temp+baseb+rtb+offsetb;
    temp.append(baseb);
    temp.append(rtb);
    temp.append(offsetb);
    return temp;

}

//for shift instructions
string shiftFormat(string line){
    string rd=line.substr(0, line.find(','));
    line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
    string rt=line.substr(0, line.find(','));
    line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
    string sa=line.substr(0, line.find(' ')-1);

    string rdb=getRegNum(rd);
    string rtb=getRegNum(rt);
    string sab=getRegNum(sa);
    string temp="";
    temp.append(rtb);
    temp.append(rdb);
    temp.append(sab);
    return temp;
}

//function to parse the instruction line to oct and then print to file
void getInstruction(string line){

    string instruction=line.substr(0, line.find(' '));
    line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
    string binLine="";

    if(instruction.compare("add")==0){
        binLine="000000";
        binLine.append(rFormat(line));
        binLine.append("00000100000");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("addi")==0){
        binLine="001000";
        binLine.append(iFormat(line));
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("and")==0){
        binLine="000000";
        binLine.append(rFormat(line));
        binLine.append("00000100100");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("andi")==0){
        binLine="001100";
        binLine.append(iFormat(line));
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("beq")==0){
        binLine="000100";
        binLine.append(iFormat(line));
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("bne")==0){
        binLine="000101";
        binLine.append(iFormat(line));
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("div")==0){
        string rs=line.substr(0, line.find(','));
        line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
        string rt=line.substr(0, line.find(' ')-1);

        string rsb=getRegNum(rs);
        string rtb=getRegNum(rt);
        string temp="";
        temp=rsb+rtb;

        binLine="000000";
        binLine.append(temp);
        binLine.append("0000000000011010");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("j")==0){
        binLine="000010";
        binLine.append(jFormat(line));
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("jal")==0){
        binLine="000011";
        binLine.append(jFormat(line));
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("jr")==0){
//////////this is special must do manually
//delete the next line
binLine="00000000000000000000000000000001";//1
binLineToHex(binLine);
textLength+=8;
    }else if(instruction.compare("la")==0){
        //pseudo code instruction
        //la $rs, big--big is the address of the variable
        //lui then ori
        //lui $at, upper( big )
        //ori $rs, $at, lower( big )

        string rs=line.substr(0, line.find(','));
        line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
        string big=line.substr(0, line.find(' '));

        int j=0;
        int mem;
        while(j<memLocation){
            string s=memStore[j];
            if(s.length()>0){
                s=s.substr(1, s.length()-1);
                if(s.compare(big)==0){
                    mem=j*4+offsetVal;
                }
            }
            j++;
        }

        stringstream number;
        number<<hex<<mem;
        string final(number.str());

        string upper=final.substr(0, 4);
        string lower=final.substr(4, 7);

        //hex to dec
        int result1 = 0;
        int pow = 1;
        for ( int i = upper.length() - 1; i >= 0; --i, pow <<= 1 )
            result1 += (upper[i] - '0') * pow;

        int result2 = 0;
        pow = 1;
        for ( int i = lower.length() - 1; i >= 0; --i, pow <<= 1 )
            result2 += (lower[i] - '0') * pow;

        string r1=to_string(result1);
        string r2=to_string(result2);

        string test1="lui $at, "+r1;
        string test2="ori "+rs+", $at, "+r2;

        getInstruction(test1);
        getInstruction(test2);
    //binLine="00000000000000000000000000000010";//2
    }else if(instruction.compare("li")==0){
        //pseudo code instruction
        //determine the size of imm
        //if a is small
        //addi $rt, $rs, imm
        //else
        //lui $at, upper( imm )
        //ori $rs, $at, lower( imm )
    binLine="00000000000000000000000000000011";//3
    binLineToHex(binLine);
    textLength+=8;
    }else if(instruction.compare("lui")==0){
        string rt=line.substr(0, line.find(','));
        line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
        string imm=line.substr(0, line.find(' ')-1);

        string rtb=getRegNum(rt);
        int immed=atoi(imm.c_str());
        string immb=decimalToBinary(immed);
        string temp="";
        temp=rtb+immb;

        binLine="00111100000";
        binLine.append(temp);
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("lw")==0){
        binLine="100011";
        binLine.append(regFormat(line));
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("mfhi")==0){
        string rd=line.substr(0, line.find(' ')-1);
        string rdb=getRegNum(rd);
        binLine="0000000000000000";
        binLine.append(rdb);
        binLine.append("00000010000");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("mflo")==0){
        string rd=line.substr(0, line.find(' ')-1);
        string rdb=getRegNum(rd);
        binLine="0000000000000000";
        binLine.append(rdb);
        binLine.append("00000010010");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("mul")==0){
        binLine="011100";
        binLine.append(rFormat(line));
        binLine.append("00000000010");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("nor")==0){
        binLine="000000";
        binLine.append(rFormat(line));
        binLine.append("00000100111");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("or")==0){
        binLine="000000";
        binLine.append(rFormat(line));
        binLine.append("00000100101");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("ori")==0){
        binLine="001101";
        binLine.append(iFormat(line));
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("sll")==0){
        binLine="00000000000";
        binLine.append(shiftFormat(line));
        binLine.append("000000");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("slt")==0){
        binLine="000000";
        binLine.append(rFormat(line));
        binLine.append("00000101010");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("sra")==0){
        binLine="00000000000";
        binLine.append(shiftFormat(line));
        binLine.append("000011");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("srl")==0){
        binLine="00000000000";
        binLine.append(shiftFormat(line));
        binLine.append("000010");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("sub")==0){
        binLine="000000";
        binLine.append(rFormat(line));
        binLine.append("00000100010");
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("sw")==0){
        binLine="101011";
        binLine.append(regFormat(line));
        binLineToHex(binLine);
        textLength+=8;
    }else if(instruction.compare("syscall")==0){
//////////this is special must do manually
        //binLine="000000"+code+"001100";
//delete the next line
binLine="00000000000000000000000000001000";//8
binLineToHex(binLine);
textLength+=8;
    }else{
        //error, instruction not supported
        binLine="00000000000000000000000000001001";//9
        binLineToHex(binLine);
        textLength+=8;
    }

}

//gets the hex data
void getHexText(){
    int j=0;
    while(j<instructionLocation){
        getInstruction(instructionStore[j]);
        ++j;
    }
}

//print the text section of the o file
void pfText(){
    //print the memory address
    outFile<<"00400000"<<endl;

    //get length
    outFile << setfill ('0') << std::setw (8); //print the memory address
    outFile<<hex<<textLength<<endl;

    int j=0;
    while(j<instructionHexLocation){
        outFile<<instructionHexStore[j]<<endl;
        j++;
    }
}

//print the data sectio of the o file
void pfData(){
    outFile<<"4d49505333326f\n";//print the arbitrary number
    outFile<<"00\n";//start the data section
    outFile<<hex<<offsetVal<<endl;//print the memory data value
    int j=0;
    int len=0;
    char t=' ';
    while(j<memLocation){
        if(memStore[j].compare("")!=0){
            t=memStore[j][0];
        }
        if(t=='W'){
            len=len+8;
        }
        if(t=='Z'){
            len=len+2;
        }
        ++j;
    }
    outFile << setfill ('0') << std::setw (8);
    outFile<<hex<<len;
    j=0;
    while(j<memLocation){
        if(memStore[j].compare("")!=0){
            t=memStore[j][0];
            outFile<<endl;
        }
        if(t=='W'){
            int num = atoi(memValues[j].c_str());
            outFile << setfill ('0') << std::setw (8);
            outFile<<hex<<num;
        }
        if(t=='Z'){
            int num = (int)memValues[j][0];
            if(memValues[j].compare("")==0){
                num=0;
                outFile << setfill ('0') << std::setw (2);
                outFile<<hex<<num;
            }else
            if(memValues[j].compare("\\n")==0){
                num=10;
                outFile << setfill ('0') << std::setw (2);
                outFile<<hex<<num;
            }else
            if(memValues[j].compare("\\0")==0){
                num=NULL;
            }else{
                outFile << setfill ('0') << std::setw (2);
                outFile<<hex<<num;
            }




        }
        ++j;
    }

}

//send the text section to an array and the branch tags with appropriate addresses
void textToMemory(string line){
    int test=line.find(':');
    if(test<0){
        line=line.substr(0, line.find('#'));
        instructionStore[instructionLocation]=line;
        instructionMemAdd[instructionLocation]=instructionVal+instructionLocation*4;
        instructionLocation++;
    }else{
        branchStore[branchLocation]=line;
        brancMemAdd[branchLocation]=instructionLocation*4+instructionVal;
        branchLocation++;
    }
}

//fuction to store the values into "memory"
void dataToMemory(string line){
    string varName=line.substr(0,line.find(':'));
    line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
    string type=line.substr(line.find('.'), line.find(' ')-line.find('.'));
    line=line.substr(line.find(' ')+1, line.size()-line.find(' '));

    if(type.compare(".word")==0){//array of numbers
        memStore[memLocation]="W"+varName;
        do{
            memValues[memLocation]=line.substr(0, line.find(','));
            line=line.substr(line.find(',')+1, line.size()-line.find(','));
            ++memLocation;
        }while(line.find(',')<line.size());
    }
    if(type.compare(".asciiz")==0){//array of numbers
        memStore[memLocation]="Z"+varName;
        line=line.substr(1, line.size()-2);
        if(line.size()!=0){
            do{
                if(line[0]=='\\'){
                    memValues[memLocation]=line.substr(0,2);
                    line=line.substr(2,line.size());
                    ++memLocation;
                }else{
                    memValues[memLocation]=line[0];
                    line=line.substr(1, line.size()-1);
                    ++memLocation;
                }
                if(line.size()==0){
                    memValues[memLocation]="\\0";
                    ++memLocation;
                }
            }while(line.size()>0);
        }else{
            memValues[memLocation]="\\0";
            ++memLocation;
        }

    }
    if(type.compare(".ascii")==0){//array of numbers
        memStore[memLocation]="X"+varName;
        line=line.substr(1, line.size()-2);
        if(line.size()!=0){
            do{
                if(line[0]=='\\'){
                    memValues[memLocation]=line.substr(0,2);
                    line=line.substr(2,line.size());
                    ++memLocation;
                }else{
                    memValues[memLocation]=line[0];
                    line=line.substr(1, line.size()-1);
                    ++memLocation;
                }
                if(line.size()==0){
                    memValues[memLocation]="";
                    ++memLocation;
                }
            }while(line.size()>0);
        }else{
            memValues[memLocation]="";
            ++memLocation;
        }

    }
}

int main (int argc, const char *argv[])
{
    string buff;
    int status=0;
    char inFileName[256]="foo.s";

    //printf("Enter the source file name\n");
    //scanf("%250s", inFileName);

    inFile.open(inFileName);
    outFile.open("fooMINE.o");

    if(inFile.is_open()){
        cout<<"Sourece opened\n";
        while(getline(inFile, buff)){
            if(buff[0]=='#' || buff[0]=='\0'){

            }else{
                if(buff.compare(".data")==0){
                    status=1;
                }else
                if(buff.compare(".text")==0){
                    status=2;
                    pMemory();
                    pfData();//print data part of file
                    outFile<<endl<<"01"<<endl;
                }else
                if(status==1){
                    dataToMemory(buff);
                }else
                if(status==2){
                    textToMemory(buff);
                }else{}

            }
        }


    }else{
        cout<<"source NOT opened\n";

    }

    getHexText();
    pText();//print text array DELETE THIS
    pfText();
    int j=0;
    while(j<instructionLocation){
        getInstruction(instructionStore[j]);
        ++j;
    }

    cout<<endl<<endl<<textLength;



    outFile.close();
    inFile.close();


    return 0;
}



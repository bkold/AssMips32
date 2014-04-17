#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iomanip>

//issues at the moment

//decimalToBinary may not output the right length needed
//decimalToBinary may not do twos compliment
//creat custom parsers for the needed instructions
//creat the pseudocode instructions like la


using namespace std;

ifstream inFile;
ofstream outFile;
static string memStore[1028];
static string memValues[1028];
static int memLocation=0;
static const int offsetVal=268500992;

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
        while(length<16){
            binary="0"+binary;
            length=binary.length();
        }
  }


  return binary;
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
    }else if(reg.compare("$30")==0 || reg.compare("$s8")==0){
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
        int immed=atoi(imm.c_str());
        string immb=decimalToBinary(immed);

        return immb;
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

//function to parse the instruction line to oct and then print to file
void getInstruction(string line){
    string instruction=line.substr(0, line.find(' '));
    line=line.substr(line.find(' ')+1, line.size()-line.find(' '));
    string binLine="";
        //todo
        //determine the format for each instr manually
        //done

        /*
        */

    if(instruction.compare("add")==0){
        binLine="000000";
        binLine.append(rFormat(line));
        binLine.append("00000100000");
    }else if(instruction.compare("addi")==0){
        binLine="001000";
        binLine.append(iFormat(line));
    }else if(instruction.compare("and")==0){
        binLine="000000";
        binLine.append(rFormat(line));
        binLine.append("00000100100");
    }else if(instruction.compare("andi")==0){
        binLine="001100";
        binLine.append(iFormat(line));
    }else if(instruction.compare("beq")==0){
        binLine="000100";
        binLine.append(iFormat(line));
    }else if(instruction.compare("bne")==0){
        binLine="000101";
        binLine.append(iFormat(line));
    }else if(instruction.compare("div")==0){
/////////this is special must do manually
//delete the next line
binLine="00000000000000000000000000000000";//0
    }else if(instruction.compare("j")==0){
        binLine="000010";
        binLine.append(jFormat(line));
    }else if(instruction.compare("jal")==0){
        binLine="000011";
        binLine.append(jFormat(line));
    }else if(instruction.compare("jr")==0){
//////////this is special must do manually
//delete the next line
binLine="00000000000000000000000000000001";//1
    }else if(instruction.compare("lui")==0){
//////////this is special must do manually
//delete the next line
binLine="00000000000000000000000000000010";//2
    }else if(instruction.compare("lw")==0){
        binLine="100011";
        binLine.append(regFormat(line));
    }else if(instruction.compare("mfhi")==0){
//////////this is special must do manually
//        string rd;
        //get rd
//        binLine="0000000000000000"+rd+"00000010000";
//delete the next line
binLine="00000000000000000000000000000011";//3
    }else if(instruction.compare("mflo")==0){
//////////this is special must do manually
//        string rd;
        //get rd
//        binLine="0000000000000000"+rd+"00000010010";
//delete the next line
binLine="00000000000000000000000000000100";//4
    }else if(instruction.compare("mul")==0){
        binLine="011100";
        binLine.append(rFormat(line));
        binLine.append("00000000010");
    }else if(instruction.compare("nor")==0){
        binLine="000000";
        binLine.append(rFormat(line));
        binLine.append("00000100111");
    }else if(instruction.compare("or")==0){
        binLine="000000";
        binLine.append(rFormat(line));
        binLine.append("00000100101");
    }else if(instruction.compare("ori")==0){
        binLine="001101";
        binLine.append(iFormat(line));
    }else if(instruction.compare("sll")==0){
//////////this is special must do manually
//delete the next line
binLine="00000000000000000000000000000101";//5
    }else if(instruction.compare("slt")==0){
        binLine="000000";
        binLine.append(rFormat(line));
        binLine.append("00000101010");
    }else if(instruction.compare("sra")==0){
//////////this is special must do manually
//delete the next line
binLine="00000000000000000000000000000110";//6
    }else if(instruction.compare("srl")==0){
//////////this is special must do manually
//delete the next line
binLine="00000000000000000000000000000111";//7
    }else if(instruction.compare("sub")==0){
        binLine="000000";
        binLine.append(rFormat(line));
        binLine.append("00000100010");
    }else if(instruction.compare("sw")==0){
        binLine="101011";
        binLine.append(regFormat(line));
    }else if(instruction.compare("syscall")==0){
//////////this is special must do manually
        //binLine="000000"+code+"001100";
//delete the next line
binLine="00000000000000000000000000001000";//8
    }else{
    //error, instruction not supported
    binLine="00000000000000000000000000001001";//9
    }
    //convert the binLine to hex and print it to the file
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
            case 1010: hexLine=hexLine+"A";
            break;
            case 1011: hexLine=hexLine+"B";
            break;
            case 1100: hexLine=hexLine+"C";
            break;
            case 1101: hexLine=hexLine+"D";
            break;
            case 1110: hexLine=hexLine+"E";
            break;
            case 1111: hexLine=hexLine+"F";
            break;
        }
        count+=4;
    }
    cout<<hexLine<<endl; //delete this when done
    outFile<<hexLine<<endl;

}

//print the data sectio of the o file
void pfData(){
    outFile<<"4D49505333326F\n";//print the arbitrary number
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

//fuction to store the values into "memory"
void sendToMemory(string line){
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
    outFile.open("foo.o");

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
                    sendToMemory(buff);
                }else
                if(status==2){
                    getInstruction(buff);
                }
            }
        }
    }else{
        cout<<"source NOT opened\n";

    }

    pMemory();

    outFile.close();
    inFile.close();


    return 0;
}



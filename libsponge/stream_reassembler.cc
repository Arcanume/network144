#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`
#define Max 0x3f3f3f3f

using namespace std;


StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity),idx(0),left(0),endof(-1),qs(){
    
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    if(index<=idx&&index+data.length()>idx){
        size_t enable=min(data.length()-(idx-index),_capacity-left-_output.buffer_size();
        _output.write(data.substr(idx-index,enable));
        idx+=enable;
    }

    if(qs.empty()){
        segment temp;
        size_t enable=min(data.length(),_capacity-left-_output.buffer_size();
        temp.from=index;
        temp.to=index+enable;
        temp.next=Max;
        temp.len=enable;
        temp.str=data.substr(0,enable);
        qs.push(temp);
        left+=enable;
    }else{
        string strtemp=data;
        size_t index1=index;
        queue<segment> backup;
        while(!qs.empty()){
            segment fir=qs.front();
            qs.pop();

            if(fir.to<=index1){
                backup.push(fir);
                continue;
            }
            else if(fir.from<=index1){
                if(fir.to>=index1+strtemp.length()){
                    backup.push(fir);
                    continue;
                }
                else{
                    strtemp=strtemp.substr(fir.to-index1);
                    index1=fir.to;
                    segment seg;
                    size_t enable=min(strtemp.length(),_capacity-left-_output.buffer_size());
                    enable=min(enable,fir.next-fir.to);
                    seg.from=index1;
                    seg.to=index1+enable;
                    seg.next=fir.next;
                    seg.len=enable;
                    seg.str=strtemp.substr(0,enable);
                    fir.next=seg.from;
                    backup.push(fir);
                    backup.push(seg);
                    left+=enable;
                    if(_capacity-left-_output.buffer_size()==0||seg.next==Max)
                        break;
                    index1+=enable;
                    strtemp=strtemp.substr(enable);
                }
            }
            else{
                    segment seg;
                    size_t enable=min(strtemp.length(),_capacity-left-_output.buffer_size());
                    enable=min(enable,fir.next-index1);
                    seg.from=index1;
                    seg.to=index1+enable;
                    
                    seg.len=enable;
                    seg.str=strtemp.substr(0,enable);
                    fir.next=seg.from;
                    backup.push(fir);
                    backup.push(seg);
                    left+=enable;

            }
            if(fir.next==Max){
                
            }
        }
        //clear left
        while(!qs.empty()){
            backup.push(qs.front());
            qs.pop();
        }
        qs=backup;
    }
    //write 
    while(){

    }
    if(eof)
        endof=index+data.length();
    if(idx==endof)
        _output.end_input();
}

size_t StreamReassembler::unassembled_bytes() const { return left; }

bool StreamReassembler::empty() const { return left==0; }

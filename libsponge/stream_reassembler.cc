#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`
#define Max 0x3f3f3f3f
#include<iostream>

using namespace std;


StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity),idx(0),left(0),endof(-1),qs(){

}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {

    segment tp(data,index,index+data.length());
    qs.push_back(tp);
    sort(qs.begin(),qs.end(),less<segment>());

    size_t tmp=0;
    /*
    if(idx==3){
        for(size_t i=0;i<qs.size();i++){
            cout<<i<<" "<<qs[i].from<<' '<<qs[i].to<<' '<<qs[i].str<<endl;
        }
    }*/
    while(qs.size()&&idx>qs[0].from){
        if(qs[0].to<=idx){
            qs.erase(qs.begin());
            continue;
        }
        qs[tmp].str=qs[tmp].str.substr(idx-qs[tmp].from);
        qs[tmp].from=idx;
        tmp++;
    }
    /*
    if(idx==3){
        for(size_t i=0;i<qs.size();i++){
            cout<<i<<" "<<qs[i].from<<' '<<qs[i].to<<' '<<qs[i].str<<endl;
        }
    }
    */
    size_t k=0;
    left=0;
    size_t enable0=min(qs[0].str.length(),_capacity-_output.buffer_size());
    qs[0].str=qs[0].str.substr(0,enable0);
    qs[0].to=qs[0].from+enable0;
    qs[k]=qs[0];
    left+=enable0;

    for(size_t i=1;i<qs.size();i++){
        if(qs[i].from==qs[k].from){
            continue;
        }
        if(qs[i].to<=qs[k].to){
            continue;
        }
        if(qs[i].from<qs[k].to){
            qs[i].str=qs[i].str.substr(qs[k].to-qs[i].from);
            size_t enable1=min(qs[i].str.length(),_capacity-_output.buffer_size()-left);
            qs[k].str=qs[k].str+qs[i].str.substr(0,enable1);
            qs[k].to+=enable1;
            left+=enable1;
        }else{
            k++;
            size_t enable2=min(qs[i].str.length(),_capacity-_output.buffer_size()-left);
            qs[i].str=qs[i].str.substr(0,enable2);
            qs[i].to=qs[i].from+enable2;
            qs[k]=qs[i];
            left+=enable2;
        }
    }
    while(qs.size()>(k+1)){
        qs.pop_back();
    }
    /*
    if(idx==3){
        for(size_t i=0;i<qs.size();i++){
            cout<<i<<" "<<qs[i].from<<' '<<qs[i].to<<' '<<qs[i].str<<endl;
        }
    }
    */
    while(qs.size()&&idx>=qs[0].from){
        if(qs[0].to<=idx){
            left-=qs[0].to-qs[0].from;
            qs.erase(qs.begin());
            continue;
        }
        size_t enab=idx-qs[0].from;
        _output.write(qs[0].str.substr(enab));
        left-=qs[0].to-qs[0].from;
        idx+=qs[0].to-qs[0].from;
        qs.erase(qs.begin());
    }
    if(eof)
        endof=index+data.length();
        //cout<<"idx:"<<idx<<"\teof :"<<endof<<endl;
    
    if(idx==endof)
        _output.end_input();

}

size_t StreamReassembler::unassembled_bytes() const { return left; }

bool StreamReassembler::empty() const { return left==0; }

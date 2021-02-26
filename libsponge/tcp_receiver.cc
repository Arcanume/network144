#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.
#include <iostream>
template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

void TCPReceiver::segment_received(const TCPSegment &seg) {
   bool first=false;
    if(seg.header().syn){
        syn_recv=true;
        isn=seg.header().seqno;
        _reassembler.acknum()++;
        first=true;
    }
    if(syn_recv&&seg.header().fin){
        fin_recv=true;
    }
    
    if(syn_recv){
        uint64_t checkpoint=_reassembler.acknum();
        checkpoint--;
        //cout<<_reassembler.acknum()<<endl;
        uint64_t idx=unwrap(seg.header().seqno,isn,checkpoint);
        if(first)
            idx++;
        //convert payload to string?
        //cout<<"acknum:"<<_reassembler.acknum()<<"\tidx"<<idx<<endl;
        _reassembler.push_substring(static_cast<string>(seg.payload().str()),idx,fin_recv);
        fin_recv=false;
        //cout<<static_cast<string>(seg.payload().str())<<endl<<static_cast<string>(seg.payload().str()).length()<<endl;
        //cout<<"acknum:"<<_reassembler.acknum()<<endl;
        bool fini=stream_out().input_ended();
        if(fini){
            _reassembler.acknum()++;
        }
        //cout<<"acknum:"<<_reassembler.acknum()<<endl;
    }
}

optional<WrappingInt32> TCPReceiver::ackno() const { 
    if(!syn_recv)
        return {};
    uint64_t siz=_reassembler.acknum();//+(fin_recv?1:0);//(syn_recv?1:0)+
    return wrap(siz,isn);
 }

size_t TCPReceiver::window_size() const {
    return _capacity-stream_out().buffer_size();
}

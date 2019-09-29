import serial, os, math, argparse, struct
from time import sleep

BLOCK_SIZE = 512 # 128 for parts with >8k flash


def sendCmdPacket(ser,cmd,cmdData=[]):
    frameLen=1+len(cmdData)
    frameBytes=struct.pack('<HHBs',0x776e,frameLen,cmd,cmdData)
    ser.write(frameBytes)
    ser.flush()

def recvCmdReply(ser,cmd):
    frameHeader = ser.read(4)
    if len(frameHeader)!=4:
        print('Invalid ACK Len!')
        return []
    else:
        frameId,frameLen=struct.unpack('<HH',frameHeader) 
        if frameId!=0x776E:
            print('Invalid Frame ID!')
            return []
    frameData= ser.read(frameLen)
    cmdReply,cmdReplyData=struct.unpack('<B%ds'%(frameLen-1),frameData)   
    if cmdReply!=cmd:
        return []
    return cmdReplyData

def readFlashSize(ser):
    sendCmdPacket(ser,0x04)
    replyData=recvCmdReply(ser,0x04)
    if replyData!=[]:
        return struct.unpack('<I',replyData)
    else:
        return 0


def bootloader_exec(port, baud):
    ser = serial.Serial(port, baud, timeout=1.0)
    flashSize=readFlashSize(ser)
    print('Flash size: ',flashSize)
    data = open(FILE, 'rb')
    total = 0
    with data as f:
        flash_cmd = 0x02
        chunk = bytearray(f.read(BLOCK_SIZE))
        chunkIndex=0
        while chunk:
            frame = bytearray(EVENT_FRAME)
            total += len(chunk)
            if total>flashSize:
                print('Exceed flash size!!')
                break
            print('Written: ',total)
            frameSize=len(chunk)+5
            frame.extend([frameSize&0xFF,frameSize>>8,flash_cmd,chunkIndex&0xff,chunkIndex>>8,len(chunk)&0xff,len(chunk)>>8])
            frame.extend(chunk)
            ser.write(frame)
            ser.flush()
            chunk = bytearray(f.read(BLOCK_SIZE))
            ack = ser.read(9)
            if len(ack)!=9:
                print('Invalid ACK Len!')
                break            
            frameId,frameLen,cmd,blockIndex,blockLen=struct.unpack('<HHBHH',ack)
            if frameId!=0x776E:
                print('Invalid Frame ID!')
                break
            if cmd==flash_cmd:
                if blockIndex!=chunkIndex:
                    print('Block Index not matched.')
                    break
            chunkIndex+=1
    ser.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='stm8-bootloader update utility')
    parser.add_argument('--port', '-p', default='COM6')
    parser.add_argument('--baud', '-b', default=115200)
    parser.add_argument('file', help='firmware in binary format')
    args = parser.parse_args()
    FILE = args.file
    bootloader_exec(args.port, args.baud)

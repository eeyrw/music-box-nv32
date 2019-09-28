import serial, os, math, argparse, struct
from time import sleep

BLOCK_SIZE = 512 # 128 for parts with >8k flash

EVENT_FRAME = (0x6e,0x77)
FILE = None


def readFlashSize(port, baud):
    ser = serial.Serial(port, baud, timeout=1.0)
    ser.write([0x6e,0x77,0x01,0x00,0x04])
    ser.flush()
    ack = ser.read(9)
    if len(ack)!=9:
        print('Invalid ACK Len!')
        return 0            
    frameId,frameLen,cmd,flashDataSize=struct.unpack('<HHBI',ack)
    if frameId!=0x776E:
        print('Invalid Frame ID!')
        return 0  
    if cmd==0x04:
        print('Block Index not matched.')
        return flashDataSize

def bootloader_exec(port, baud):
    flashSize=readFlashSize(port,baud)
    print('Flash size: ',flashSize)
    ser = serial.Serial(port, baud, timeout=1.0)
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

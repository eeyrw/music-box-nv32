import serial, os, math, argparse
from time import sleep

BLOCK_SIZE = 512 # 128 for parts with >8k flash

EVENT_FRAME = (0x6e,0x77)
FILE = None




def bootloader_exec(port, baud):
    ser = serial.Serial(port, 115200, timeout=1.0)
    data = open(FILE, 'rb')
    total = 0
    with data as f:
        flash_cmd = 0x02
        chunk = bytearray(f.read(BLOCK_SIZE))
        chunkIndex=0
        while chunk:
            frame = bytearray(EVENT_FRAME)
            total += len(chunk)
            print(total)
            frameSize=len(chunk)+5
            frame.extend([frameSize&0xFF,frameSize>>8,flash_cmd,chunkIndex&0xff,chunkIndex>>8,len(chunk)&0xff,len(chunk)>>8])
            frame.extend(chunk)
            ser.write(frame)
            ser.flushOutput()
            chunk = bytearray(f.read(BLOCK_SIZE))
            ack = ser.read(9)
            print('Recv: ',ack.hex())
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

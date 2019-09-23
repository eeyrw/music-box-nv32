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
        frame = bytearray(EVENT_FRAME)
        flash_cmd = 0x02
        chunk = bytearray(f.read(BLOCK_SIZE))
        chunkIndex=0
        while chunk:
            total += len(chunk)
            print(total)
            chunk.extend([0xFF] * (BLOCK_SIZE - len(chunk)))
            frame.extend([flash_cmd,5,chunkIndex&0xff,chunkIndex>>8,BLOCK_SIZE&0xff,BLOCK_SIZE>>8,chunk])
            ser.write(frame)
            ser.flushOutput()
            chunk = bytearray(f.read(BLOCK_SIZE))
        ack = ser.read(1)
        if ack == bytearray(0x02):
            print('Done')
        else:
            print('Invalid response')
    ser.close()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='stm8-bootloader update utility')
    parser.add_argument('--port', '-p', default='COM6')
    parser.add_argument('--baud', '-b', default=115200)
    parser.add_argument('file', help='firmware in binary format')
    args = parser.parse_args()
    FILE = args.file
    bootloader_exec(args.port, args.baud)

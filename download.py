import serial
import os
import math
import argparse
import struct
from time import sleep
import serial.tools.list_ports

BLOCK_SIZE = 512  # 128 for parts with >8k flash


def autoFindPort():
    portList = serial.tools.list_ports.comports()
    pid = "7523"
    vid = "1A86"
    port = None
    for p in portList:
        if pid and vid in p.hwid:
            port = p.device
    return port


def sendCmdPacket(ser, cmd, cmdData=b''):
    frameLen = 1+len(cmdData)
    frameBytes = struct.pack('<HHB%ds' % len(
        cmdData), 0x776e, frameLen, cmd, cmdData)
    ser.write(frameBytes)
    ser.flush()


def recvCmdReply(ser, cmd):
    frameHeader = ser.read(4)
    if len(frameHeader) != 4:
        print('Invalid ACK Len!')
        return b''
    else:
        frameId, frameLen = struct.unpack('<HH', frameHeader)
        if frameId != 0x776E:
            print('Invalid Frame ID!')
            return b''
    frameData = ser.read(frameLen)
    cmdReply, cmdReplyData = struct.unpack('<B%ds' % (frameLen-1), frameData)
    if cmdReply != cmd:
        return b''
    return cmdReplyData


def readFlashSize(ser):
    sendCmdPacket(ser, 0x04)
    replyData = recvCmdReply(ser, 0x04)
    if replyData != b'':
        return struct.unpack('<I', replyData)[0]
    else:
        return 0


def startFlash(ser):
    sendCmdPacket(ser, 0x01)
    replyData = recvCmdReply(ser, 0x01)
    if replyData == b'':
        return True
    else:
        return False


def endFlash(ser):
    sendCmdPacket(ser, 0x03)
    replyData = recvCmdReply(ser, 0x03)
    if replyData == b'':
        return True
    else:
        return False


def bootloader_exec(port, baud):
    ser = serial.Serial(port, baud, timeout=2.0)
    startFlash(ser)
    flashSize = readFlashSize(ser)
    print('Flash size: ', flashSize)
    data = open(FILE, 'rb')
    total = 0
    with data as f:
        flash_cmd = 0x02
        chunkIndex = 0
        chunk = bytearray(f.read(BLOCK_SIZE))
        while chunk:
            total += len(chunk)
            if total > flashSize:
                print('Exceed flash size!!')
                break
            print('Written: ', total)
            cmdData = bytearray(struct.pack('<HH', chunkIndex, len(chunk)))
            cmdData.extend(chunk)
            sendCmdPacket(ser, flash_cmd, cmdData)
            replyData = recvCmdReply(ser, flash_cmd)
            recvChunkIndex, recvChunkLen = struct.unpack('<HH', replyData)
            if recvChunkIndex != chunkIndex or recvChunkLen != len(chunk):
                print('Wrong ACK!')
                break
            chunkIndex += 1
            chunk = bytearray(f.read(BLOCK_SIZE))
    endFlash(ser)
    ser.close()


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description='music box score data downloader')
    port = autoFindPort()
    if port == None:
        port = 'COM6'
    parser.add_argument('--port', '-p', default=port)
    parser.add_argument('--baud', '-b', default=115200)
    parser.add_argument('file', help='firmware in binary format')
    args = parser.parse_args()
    FILE = args.file
    print('Serial Port: ',port)
    bootloader_exec(args.port, args.baud)

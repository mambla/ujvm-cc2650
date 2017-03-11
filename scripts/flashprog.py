import argparse
import serial
import serial.tools.list_ports
import struct
import os
import time

CHUNK_SIZE = 1024


def main():
	parser = argparse.ArgumentParser()
	parser.add_argument('file_path')
	args = parser.parse_args()
	file_path = args.file_path

	with open(file_path, 'rb') as f:
		with serial.Serial(
				port=get_serial_port(),
				baudrate=115200,
				bytesize=serial.EIGHTBITS,
				stopbits=serial.STOPBITS_ONE,
				parity=serial.PARITY_NONE) as port:
			file_size = os.path.getsize(file_path)
			# invoke_print(port, file_size)
			# time.sleep(0.5)
			write_file_to_serial_port(port, f, file_size)
			time.sleep(0.5)
			# invoke_print(port, file_size)
			# time.sleep(1)
			invoke_java(port)


def invoke_print(port, size):
	port.write('p')

	if port.read(1) != 'p':
		raise Exception('Board not ready for printing')

	print 'Board ready for printing'

	time.sleep(0.1)

	port.write(struct.pack('I', size))


def invoke_java(port):
	port.write('r')


def write_file_to_serial_port(port, file, size):
	port.write('w')

	if port.read(1) != 'w':
		raise Exception('Board not ready for programming')

	print 'Board ready for programming'

	time.sleep(0.1)
	port.write(struct.pack('II', size, CHUNK_SIZE))

	if port.read(1) != 'r':
		raise Exception('Board didnt receive file and chunk size')

	print 'Board received file (%d) and chunk (%d) size' % (size, CHUNK_SIZE)

	i = 0

	while True:
		data = file.read(CHUNK_SIZE)

		if data == "":
			break

		print 'Sending chunk %d' % i

		time.sleep(0.1)
		port.write(data)

		print 'Waiting for ack'

		if port.read(1) != 'c':
			raise Exception('Board didnt receive chunk')

		print 'Sent chunk %d' % i

		i += 1

	if port.read(1) != 'f':
		raise Exception('Board didnt receive all data')

	print 'Done!'


def get_serial_port():
	for port in serial.tools.list_ports.comports():
		if 'User UART' in port.description or 'ACM0' in port.device:
			print 'Found serial device: %s' % port.device
			return port.device

	raise Exception('Failed to find UART port')


if __name__ == '__main__':
	main()

import argparse
import glob2
import os
import subprocess
import struct


def main():
	parser = argparse.ArgumentParser()
	parser.add_argument('--class_cvt')
	parser.add_argument('class_root_dir')
	parser.add_argument('out_path')

	args = parser.parse_args()

	class_files = glob2.glob(os.path.join(args.class_root_dir, '**', '*.class'))
	out_data = ''

	CLASSES_TO_SKIP = ['MiniString.class', 'RT.class', 'UC.class']

	for class_file in class_files:
		class_data = get_class_data(class_file, args.class_cvt)

		if os.path.basename(class_file) in CLASSES_TO_SKIP:
			print 'Skipping class %s' % class_file
			continue

		out_data += struct.pack('>I', len(class_data))
		out_data += class_data

		print 'Added class %s' % class_file

	out_data += struct.pack('>I', 0)

	with open(args.out_path, 'wb') as f:
		f.write(out_data)

	print 'Done! (%d bytes)' % len(out_data)


def get_class_data(class_path, class_cvt):
	if class_cvt is None:
		with open(class_path, 'rb') as f:
			return f.read()

	return subprocess.check_output('%s < %s' % (class_cvt, class_path), shell=True)


if __name__ == '__main__':
	main()
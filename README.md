# steganography
Steganography is the process of hiding information in an image file. In my project, I use .bmp images to encode the data because they are uncompressed and easy to encode.
format for encoding: ./a.out -e <example.bmp> <secret.txt>
Here, ./a.out is to run the program, and -e is for telling that we are doing encoding. in that .bmp image, we are storing the data taken from the secret.txt file.
format for decoding: ./a.out -d <stego_image.bpm> <output.txt>(optional)
Here, ./a.out is to run the program, and -d is for telling that we are doing decoding. That stego_image.bmp is a data-encoded image that we have to decode,
and output.txt (optional) is the file where the decoded data from the image is finally stored. if we don't write the output file name, our code will create the output file
with file extension according to the file extension data encoded in the image.

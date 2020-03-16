# Example usage: 
# python3 csv_to_wav.py -csvfile '../csv/output.csv' -wavfile
#  '../wav/output.wav' -samplerate 44100

from scipy.io import wavfile
import numpy
import csv
import argparse

# Handle command line arguments
parser = argparse.ArgumentParser()
parser.add_argument("-csvfile", help="csv file to convert to wav", 
                    type=str, required=True)
parser.add_argument("-wavfile", help="wav file from csv",
                    type=str, required=True)
parser.add_argument("-samplerate", help="sample rate of the wav file",
                    type=int, required=True)
args = parser.parse_args()

with open(args.csvfile, newline='') as f:
    reader = csv.reader(f)
    
    # Contents are returned as [['value1', 'value2', ...]]
    contents = list(reader)

    # Get the inner list of contents
    content = list(contents[0])
    
    # Last item is '', remove it
    content.pop()

    # Convert the list of strings to a list of ints
    samples = [int(i) for i in content]

# wavfile.write expects a numpy array, convert to numpy array
data = numpy.asarray(samples, dtype=numpy.int16)

# Write the data to the wavfile
wavfile.write(args.wavfile, args.samplerate, data)

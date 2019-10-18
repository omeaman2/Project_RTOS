from scipy.io.wavfile import read

def calc_distances(sound_file):
    # Read in data
    # I take it that fs is the sampling frequency.
    # Average number of samples obtained in one second, thus fs = 1/T
    fs, data = read(sound_file)

    # we will use the size of the array
    # to determine the duration of the sound
    data_size = len(data)

    # The minimum value for the sound to be recognized as a knock.
    min_val = 5000
    # The number of indexes on 0.15 seconds
    focus_size = int(0.15 * fs)

    # Juice of the algorithm
    focuses = []
    distances = []
    i = 0
    while i < len(data):
        # Look for a value exceeding our minimum value to be considered a peak.
        if data[i] > min_val:
            # We found such a value. Consider this value and any value for the
            # next 0.1 seconds as one individual knock.
            mean_index = i + focus_size // 2
            focuses.append(float(mean_index) / data_size)
            if len(focuses) > 1:
                last_focus = focuses[-2]
                actual_focus = focuses[-1]
                distances.append(actual_focus - last_focus)
                i += focus_size
        else:
            i += 1
    return distances
print(calc_distances("knock.wav"))

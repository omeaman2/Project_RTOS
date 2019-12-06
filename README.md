Project RTOS
---

Day notes are added at the end of this file. TODO is up here for clarity.

TODO
---
 - [✓] Edit the signal and look at the inverse fourier.
 - [✓] Use better comparison of floating point numbers using the epsilon
   technique. Currently when I use `(double) i` for the imaginary part of the
   number the numbers do not compare correctly.
 - [✓] Test if the ifft produces the original signal.
 A: In a hacky way :-).
 - [✓] Test if KissFFT works on FreeRTOS.
 - [✓] Add KissFFT source (perhaps as submodule) to this git.
 - [✓] Check KissFFT licensing and with regard to copying of code.
 A: Can be used just fine.


All information from last school year
---

> Arbitrary information
Projectkaders RTES

Er wordt gebruik gemaakt van een client-server software-architectuur, en die mag
gedistribueerd zijn over meerdere hardwarecomponenten.
"state-driven development"
> end

[The algorithm used for noise cancelling](https://stackoverflow.com/questions/6503562/which-algorithm-is-used-for-noise-canceling-in-earphones)
[Information on noise control](https://en.wikipedia.org/wiki/Active_noise_control)
[A chip we might need](https://en.wikipedia.org/wiki/Digital_signal_processor)

# Sun Feb 10 15:28:14 CET 2019
Current plan is to create a noise cancelling head phone with a realtime OS.

Might want to research into low and high pass filters.
These low and high pass filters can be used when we know what type of noise we
want to reduce beforehand, when we don't know we are better off using adaptive
filters.

Adaptive Filters algorithms:
 - Least mean squares
 - Recursive least squares

On the Least Mean Squares algorithm:
> The goal of the LMS method is to find the filter coefficients required to
> reduce the mean square error of the error signal [8]. The error signal is the
> difference between the desired d(n) and output signal y(n).


For project we basically want to receive the signal with noise in software, so
we need to analog to digital convert the signal, invert the signal and execute
algorithms to cancel the signal, and output the new signal without noise.

We of course need fast hardware to convert between analog and digital signals,
for that [this](https://en.wikipedia.org/wiki/Digital_signal_processor) looks
interesting.

Things to study:

 - Passive and active noise canceling

 - Hardware to use

    + Headphone

    + Microphone

    + Placement of the microphone

    + [Digital Signal Processor](https://en.wikipedia.org/wiki/Digital_signal_processor)

    + [Operational Amplifier](https://en.wikipedia.org/wiki/Operational_amplifier)

    + Power source for noise canceling

 - Algorithm to use

    + Least Mean Square (LMS)

    + Normalized Least Mean Square

    + Recursive Least Squares (RLS)

    + [Fast Fourier transform](https://en.wikipedia.org/wiki/Fast_Fourier_transform)

    + Specially look into the best algorithm to use for a realtime system (Maybe
      we have to trade speed for accuracy since our focus is a realtime system).

 - Adaptive filters
    + [Wiener filter](https://en.wikipedia.org/wiki/Wiener_filter)

    + FIR and IIR filters

 - Realtime OS to use

 - Programming
    + Programming language to use for simulation (MATLAB, Go, R, C++, Python?)

    + Programming language to use for the real deal.

Questions to answer:
 - What is our goal? How much ambient noise do we want to reduce?

 - Do we want to filter noise ranging in many frequencies or some general noise
   like the fan of a PC (or both)?

 - What is the best algorithm to use (LMS|NLMS|RLS) for our realtime
   application?

 - Why do we need a realtime OS?

# Tue Feb 12 13:38:37 CET 2019

Subjects:

 - Electronical Components to use

 - Digital Signal Processor || Controller (Embedded System)

 - Algorithm (LMS|NLMS|RLS|...)

 - Realtime (OS)

## Assignment
 - Introduce a something has to be done periodically.
 - Argue in a research subquestion why we would want to use a realtime OS for
   something.

# Sun Feb 17 18:54:29 CET 2019
 - GNU Octave seems interesting, open source MATLAB clone.

# Mon Feb 25 23:12:50 CET 2019
 - TODO: Review research questions and research elaboration, see `README.md`.

# Sun Mar  3 17:02:43 CET 2019

I want to improve on the subquestions.

We should make the subquestions so that they all produce a part of the final
result. It would also be good if the subquestions could be executed in parallel;
this would enhance the speed at which we will get to the final result as a
group.

# Tue Mar  5 15:34:43 CET 2019

## Assignment

TODO:
- European margin
- Measurable principal research question (dB)
- focus op wat je gaat onderzoeken in de hoofdvraag.
- subquestion 2
2 volgt uit 1
- define periodic (how many times per)?
- separate chapter for methods. cmobination of methods.

1 als theortisch kader als allee begrippen worden uitgelegd, depends

deelvraag:
wat dient er onderzocht te worden
wat heb je aan het antwoord

We've got a part of the probleemstelling now.

# Sun Mar 17 14:13:27 CET 2019

- [] Write problem analysis

## Kind of problem analysis and research questions

 Noise is a form of sound that is perceived as unpleasant or disturbing by the
 listener. From a physics point of view noise is not any different from a
 'regular' sound. Our brains decide for us what is perceived as noise.

 A noise sound can be a sound that is constantly present, both changing or
 unchanging in frequency, or a sound that keeps repeating itself after a certain
 amount of time has passed.

 In our research we want to find an effective way to reduce noise, with a focus
 on a noise sound that is reoccurring at a certain interval, a periodic noise.
 We want to reduce our perceived noise signal by at least six decibels, which
 should make the noise at least four times less loud as the original sound to
 the listener. We want to create a test setup in which we can measure the
 reduction of the noise sound. This test setup will consist of the necessary
 hardware and software to realise the system, including microphones to measure
 the reduction of the noise sound and headphones.

 From this description we can deduce our principal research question. Our
 principal research question is as follows:

    How can noise emitted from a periodic signal source be reduced by 6 dB?

 It is important to determine how noise cancellation works in general. There are
 various existing papers on the subject. To find out how we can reduce a noise
 signal we want to find an answer to the following subquestion:

    How does noise cancellation work?

 We want to determine what makes a periodic signal different from a normal
 signal.

    How can differentiate a periodic signal from a normal signal?

 Research has to be put in what hardware is needed to reduce a periodic noise
 signal. The hardware should be fast at signal processing; little delay is
 tolerated. Too much delay and the noise signal would already be heard by the
 listener. For the same reason the hardware should be fast at computing the
 complementary signal, which is required  to cancel the noise. To find answers
 for these requirement we will have to find an answer to the subquestion:

    Which hardware setup is required to achieve 6 dB noise reduction?

 On software level, there exist various noise cancellation algorithms. In our
 research we focus on reducing a periodic noise signal and we want to find the
 most efficient algorithm for that, so we want to find an answer to the
 subquestion:

    What noise cancelling algorithm(s) can be used reduce a periodic noise?



List of unanswered questions that should probably be answered in above text:

 * The period of the noise signal needs to be specified and substantiated.

Continue in `README.md`

# Thu Mar 21 15:30:44 CET 2019

 - Check out tests in the 50 pages paper.

While reading the 50 pages ANC paper:

> The RLS method proves to be better than the LMS method when cancelling
> periodic noise. 

$@ p. 37$

TODO: Study the code (especially the algorithms) more.
      Maybe study GNU Octave to be able to read the syntax some more.


# Thu 25 Apr 2019 06:37:31 PM CEST

Doing more research

Adaptive filters make use of the LMS and RLS algorithms. Adaptive filters
themselves are used because in the real world one would likely not have priori
information on the noise to cancel.


" Applications can be "1-dimensional" or 3-dimensional, depending on the type of
zone to protect. Periodic sounds, even complex ones, are easier to cancel than
random sounds due to the repetition in the wave form.  "

`@` https://en.wikipedia.org/wiki/Adaptive_filter with reading.

# Sat 27 Apr 2019 04:29:35 PM CEST

Continuing to do some reading.

### Adaptive filter error

The error of an closed loop adaptive filter is the the difference between the
filter output and the desired signal.

# Sun 12 May 2019 01:01:07 PM CEST

## On writing a research methodology

[good article about it](http://expertjournals.com/how-to-write-a-research-methodology-for-your-academic-article/)

"For your academic article, when you describe and explain your chosen methods
it is very important to correlate them to your research questions and/or
hypotheses. The description of the methods used should include enough details
so that the study can be replicated by other Researchers, or at least repeated
in a similar situation or framework."

"Every stage of your research needs to be explained and justified with clear
information on why you chose those particular methods, and how they help you
answer your research question or purpose."

"As the Authors, in this section you get to explain the rationale of your
article for other Researchers. You should focus on answering the following
questions"


### Focus should be put to answering the following questions:

1. How did you collect the data or how did you generate the data?

2. Which research methods did you use?

3. Why did you choose these methods and techniques?

4. How did you use these methods for analyzing the research question or
   problem?

"The responses to these questions should be clear and precise, and the answers
should be written in past tense."



"Readers need to understand how the information was gathered or generated in a
way that is consistent with research practices in a field of study."

"The methods section should fully explain the reasons for choosing a specific
methodology or technique."

"The methods section should fully explain the reasons for choosing a specific
methodology or technique."


## Method's used

- Secondary data

- Primary data


## Reasons for choosing specific research methods

Indicate how the research approach fits with the general study.

## Data analysis methods

How you intend to analyze your results.

For explaining the data analysis methods, you should aim to answer questions,
such as:

- Will your research be based on statistical analysis?

- Will you use theoretical frameworks to help you (and your Readers) analyze a
 set of hypotheses or relationships?

- Which data analysis methods will you choose?

- Which other Authors or studies have used the same methods and should be cited
 in your academic article?


## Issues to avoid


- Irrelevant details and complicated background information that provides too
  information and does not provide accurate understanding for Readers.

- Unnecessary description and explanations of basic or well-known procedures,
  for an academic audience who already has a basin understanding of the
  study.

- For unconventional research approaches, it is important to provide accurate
  details and explain why your innovative method contributes to general
  knowledge (save more details for your Discussion / Conclusion section in
  which you can highlight your contributions).

- Research limitations and obstacles should be described in a separate section
  (Research Limitations).

- The methodology should include sources and references that support your
  choice of methods and procedures, compared to the literature review that
  provides a general outlook and framework for your study.

# Thu 16 May 2019 01:07:49 PM CEST

```tex

\section{Algorithms}
\label{sec:algorithms}

Various noise cancelling algorithms exist to cancel noise in a system. Two of the most popular algorithms are the Least Mean Squares (LMS) and the Recursive LEast Squares (RLS) algorithms.

\section{Adaptive Filter}
\label{sec:adaptive_filter}

An adaptive filter is used to filter the noise out. We use an adaptive filter,
as opposed to a non-adaptive filter, because we do not know the
characteristics of the noise beforehand. The adaptive filter updates its
parameters to produce an as accurate as possible replica of the noise which it
then subtracts from the signal containing both the desired signal and the
#noise. Figure \ref{fig:adapt_model} of Section \ref{ch:TheoBack} shows an
adaptive filter in context to the two input signals and the output speaker. in
The adaptive filter makes use of one of the algorithms mentioned in
\ref{sec:algorithms}. 

\section{LMS vs RLS}

According to a study in \ref{h_s_study} the RLS algorithm proves to be better with periodic noises than LMS.

```

## Mon 27 May 2019 01:25:53 PM CEST

- P(z) is the transfer function.

- Learning rate in neural networks is how fast a neural network replaces an old
  believe for a new one.

- in and output a `.wav`: [git](https://github.com/stephencwelch/Active-Noise-Cancellation/blob/master/LMS%20Demo%20%5BVer%200%5D.ipynb)

## Wed 29 May 2019 11:41:58 AM CEST

- Sample rate is `44100`.

- Hardware and software study paper contains implementation code for the RLS
  algorithm.


- Finite impulse response (FIR) filter is a filter whose input response (or
  response to any finite input) is of finite duration, because it settles to
  zero in finite time. This is in contrast to infinite impulse response (IIR)
  filters, which may have internal feedback and may continue to respond
  indefinitely (usually decaying).

- Apparently the Wiener filter is a type of adaptive filter.
  Actually not

Adaptive wiener filter: In an adaptive Wiener filter the error signal e[n] is
fed back to the filter weights to adjust them using a steepest-descent
algorithm.

So the difference between a Wiener filter and an adaptive filter is that (I
suppose) the error is not fed back into the filter.

- With respect to the weight vector `w`, the error e[n] can be viewed as an M
  dimensional error surface, that due to the squared error criterion, is convex
  cup (a bowl shape).


So from the hw and sw study paper I've got that the RLS algorithm is the most
useful algorithm for periodic noise, and from `Seminar_Bermudez.pdf` I've got
that NLMS is the most used adaptive filtering algorithm used in realtime
applications.

## Sun 02 Jun 2019 01:51:52 PM CEST

- Note the dimensions of noise cancellation.


Todo in the code:

- Recognize periodic signal

 + Recognize data in a signal in the first place.
 [stack overflow post on pattern recognition](https://stackoverflow.com/questions/2196124/detecting-patterns-in-waves)


Dynamic Time Warping seems good because it can cope with different speed of
repetition of the signal.

- [] Research more into a good method to recognize the periodic signal.

Look if it is has a good library in python (and C).

## Sat 22 Jun 2019 02:30:29 PM CEST

Got to finish this thing.

Keypoints to do are now:

- [] Since noise cancelling is such a wide subject, we need to very clearly
  decide what research to include in the project, and what not. We should
  certainly focus on the research that is directly relevant to our project.

- [] Get FreeRTOS working. Getting a nice simulation working on free RTOS
  should be the final goal.

- [] Get a simulation working outside of FreeRTOS.

- [] Really do integrate the realtime aspect in the project. This is the most
  important keypoint to pass it.

## Thu 27 Jun 2019 01:58:34 PM CEST


[Using this source](https://tomroelandts.com/articles/how-to-create-a-simple-low-pass-filter)
- [] Get the low pass filter working.

- [] Create the noise cancelling algorithm in code.

[Good resource](https://stackoverflow.com/questions/24920346/filtering-a-wav-file-using-python)

[Another resource](https://stackoverflow.com/questions/24920346/filtering-a-wav-file-using-python)

[On noise cancelling in general](https://stackoverflow.com/questions/6503562/which-algorithm-is-used-for-noise-canceling-in-earphones)

- [] Maybe try to continue on the algorithm itself in code. 

## Sun 30 Jun 2019 05:30:16 PM CEST

[Good source on so explaining fft](https://stackoverflow.com/questions/3694918/how-to-extract-frequency-associated-with-fft-values-in-python)

For tomorrow:

We have got the frequencies (kind of). We need to filter out some frequencies so
that we can cancel the noise. For that we have to to Fourier transform (done),
then

- [] We have to remove the non-desired frequencies.

- [] Convert back to the normal signal with the inverse Fourier transform.

## Mon 01 Jul 2019 12:33:44 PM CEST

[audio and frequency](https://sound.stackexchange.com/questions/25053/sampling-frequency-vs-audio-frequency)
```python
# The OP asks how to find the frequency in Hertz. I believe the formula is
# frequency (Hz) = abs(fft_freq * frame_rate).
```

[From here](https://stackoverflow.com/questions/19589496/frame-rate-vs-sample-rate):

"
Sample rate = number of samples / second

Frame = 1 sample from each channel (PCM)

Frame Size = Sample size * Channels

Frame Rate = frames / second.

For PCM the sample rate and the frame rate are the same since a frame consists
of a a sample from each channel.
"

## Tue 02 Jul 2019 02:01:31 PM CEST

### Assessment

Start with references to other research papers. - als voorzet / opzet

APA-Richtlijnen ClosedCaptions on blackboard, gaat al de goede kant op in het
verslag.


Figure needs cite


Projectrichtlijnen op blackboard


Eisen linken met deelvragen



## Sun 07 Jul 2019 09:38:31 PM CEST

Trying to remove frequencies from the signal, I've got the frequencies but at
what index form the fourier signal should I remove them.

## Tue 09 Jul 2019 02:28:21 PM CEST

real and imag are 0 when `cut_fourier_signal[i] = 0`.

## Wed 10 Jul 2019 03:41:08 PM CEST

- How to recognize a repeating sound in the Fourier transform?

- Trying to flatten 1000 around the minimum and maximum of the signal.


[signal stack exchange on recognizing repititive changes](https://dsp.stackexchange.com/questions/52680/identify-random-repetitive-patterns)

- Blackboard requirements

- [Somewhat easier to understand autocorrelation article in python](https://stackoverflow.com/questions/14297012/estimate-autocorrelation-using-python?noredirect=1&lq=1)

## Sun 06 Oct 2019 03:31:32 PM CEST

Setting up FreeRTOS on my Windows VM.


## Tue 08 Oct 2019 05:44:08 PM CEST


On noise cancellation with the [Fourier transform and spectral subtraction](https://dsp.stackexchange.com/questions/9054/removing-noise-from-audio-using-fourier-transform-in-matlab/9056)

## Fri 18 Oct 2019 01:55:01 PM CEST

Fourier transform is hard to understand.

The current code contains only a `fft` and not an `ifft` function. The ifft will have to be created by
ourselves. To do so it's required that we fully understand how the fft
works, however.

We could also try to find a library that does have an ifft, however most
libraries are bit and presumambly hard to integrate with FreeRTOS.

[Someone using KissFFT on a wav file](https://stackoverflow.com/questions/35735803/using-kissfft-on-a-wave-file)


KissFFT works wihtout a complicated Makefile, yay :D
If KissFFT works on FreeRTOS we are not required to fully understand the `fft`
function. We would only have to understand its input and output.

## Compilation of the library

Just add the files from the library to the `gcc` command.
e.g. with a file where the following is included.

```c
#include <math.h>
#include "kissfft/kiss_fft.h"
int main(void) { /* code */ }
```

The compilation command should look like this:

```c
gcc -lm main.c kissfft/kiss_fft.{c,h}
```

Where `-lm` is used to link the `math.h` library.

## Sun 20 Oct 2019 03:29:37 PM CEST

How will we validate that _things_ work?

Do we need to plot the signal the input and output signals?

Plotting does not need to work on FreeRTOS, we can simply plot and test on our
own OS.

## Fri 06 Dec 2019 01:34:16 PM CET

Getting `not a number`

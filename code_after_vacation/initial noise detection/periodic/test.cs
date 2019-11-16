// using System;
// using System.IO;

// namespace AutoCorrelation {
//     public class AutoCorrelation {
//         public static void Main() {
//             Console.WriteLine("Processing input file...");

//             string[] s = File.ReadAllLines("t.txt");
//             double[] x = new double[s.Length ];

//             for(int j =0; j<s.Length; j++) {
//                 x[j] = double.Parse(s[j]);
//                 if(j % 1000==0)
//                 Console.WriteLine(j);
//             }

//             Console.WriteLine("Computing Autocorrelation...");
//             var q = AutoCorrelation.GetAutoCorrelationOfSeries(x);
//             File.Delete("result.txt");

//             for (int i = 0; i < q.Length; i++){
//                 Console.WriteLine(q[i]);
//                 File.AppendAllText("result.txt", q[i].ToString() + "\r\n");
//             }
            
//             Console.WriteLine("DONE");
//         }

//         public static double GetAverage( double[] data ) {
//             int len = data.Length;

//             if ( len == 0 ){ 
//                 throw new Exception("No data");
//             }

//             double sum = 0;

//             for ( int i = 0; i < data.Length; i++ ){
//                 sum += data[i];
//             }

//             return sum / len;
//         }

//         public static double GetVariance( double[] data ) {
//             int len = data.Length;

//             // Get average
//             double avg = GetAverage( data );

//             double sum = 0;

//             for ( int i = 0; i < data.Length; i++ ){
//                 sum += System.Math.Pow( ( data[i] - avg ), 2 );
//             }

//             return sum / len;
//         }
        
//         public static double GetStdev( double[] data ) {
//             return Math.Sqrt( GetVariance( data ) );
//         }

//         public static double GetCorrelation( double[] x, double[] y) {
//             if ( x.Length != y.Length ){
//                 throw new Exception("Length of sources is different");
//             }

//             double avgX = GetAverage( x );
//             double stdevX = GetStdev( x );
//             double avgY = GetAverage( y );
//             double stdevY = GetStdev( y );
//             double covXY = 0;
//             double pearson = 0;
//             int len = x.Length;

//             for ( int i = 0; i < len; i++ ){
//                 covXY += ( x[i] - avgX ) * ( y[i] - avgY );
//             }

//             covXY /= len;
//             pearson = covXY / ( stdevX * stdevY );
         
//             return pearson;
//         }

//         public static double[] GetAutoCorrelationOfSeries (double[] x) {
//             int half = (int) x.Length/2;
//             double[] autoCorrelation = new double[half];
//             double[] a = new double[half];
//             double[] b = new double[half];

//             for (int i = 0; i < half; i++) {
//                 a[i] = x[i];
//                 b[i] = x[i + i];
//                 autoCorrelation[i] = GetCorrelation(a, b);
                
//                 if(i%1000==0){
//                     Console.WriteLine(i);
//                 }
//             }
//             return autoCorrelation;
//         }

//             /* We could also do this in .NET 4.0 using the Task Parallel library as follows: */

//         // public static double[] GetAutoCorrelationOfSeries(double[] x) {
//         //     int half = (int) x.Length / 2;
//         //     Task[] tasks = new Task[half];
//         //     double[] autoCorrelation = new double[half];
//         //     double[] a = new double[half];
//         //     double[] b = new double[half];
            
//         //     for (int i = 0; i < half; i++) {
//         //         a[i] = x[i];
//         //         b[i] = x[i + i];

//         //         var task = Task.Factory.StartNew(() => DoSomeWork(a, b), TaskCreationOptions.LongRunning);
//         //         tasks[i] = task;
//         //         autoCorrelation[i] = task.Result;

//         //         if (i % 1000 == 0){
//         //             Console.WriteLine(i);
//         //         }
//         //     }

//         //     Task.WaitAll(tasks);
//         //     return autoCorrelation;
//         // }

//         // public static double DoSomeWork(double[] a, double[] b){
//         //     return GetCorrelation(a, b);
//         // }
//     }
// }

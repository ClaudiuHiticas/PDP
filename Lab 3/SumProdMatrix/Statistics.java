import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.text.DecimalFormat;

public class Statistics {

    private  int NORMAL_WAY = 1;
    private  int ASYNC_WAY = 2;
    private  int POOL_WAY = 3;
    int numberOfThreadsDone = 0;
    Boolean completed = false;

    private double mainAddition(int numberOfLines, int numberOfColumns, int numberOfThreads, int theWay)
    {
        int numberOfElements = numberOfLines * numberOfColumns;
        int maxNumberOfElementsPerThread;
        if (numberOfElements % numberOfThreads == 0)
        {
            maxNumberOfElementsPerThread = numberOfElements / numberOfThreads;
        }
        else
        {
            maxNumberOfElementsPerThread = numberOfElements / numberOfThreads + 1;
        }

        ExecutorService pool = Executors.newFixedThreadPool(numberOfThreads);
        MatrixSum matrixSum = new MatrixSum(numberOfLines,numberOfColumns,maxNumberOfElementsPerThread);
        double start = System.currentTimeMillis();

        for (int i=0; i<numberOfThreads; i++)
        {
            if(theWay == POOL_WAY)
                pool.execute(matrixSum);
            else if(theWay == ASYNC_WAY)
                CompletableFuture.runAsync(matrixSum::run);

        }
        if(theWay == POOL_WAY)
            pool.shutdown();

        double finish = System.currentTimeMillis();

        return finish - start;
    }


    private double mainMultiplication(int numberOfLinesA, int numberOfColumnsALinesB, int numberOfColumnsB, int numberOfThreads, int theWay)
    {
        int numberOfElements = numberOfLinesA * numberOfColumnsB;
        int maxNumberOfElementsPerThread;
        if (numberOfElements % numberOfThreads == 0)
        {
            maxNumberOfElementsPerThread = numberOfElements / numberOfThreads;
        }
        else
        {
            maxNumberOfElementsPerThread = numberOfElements / numberOfThreads + 1;
        }

        numberOfThreadsDone = 0;
        completed = false;
        ExecutorService pool = Executors.newFixedThreadPool(numberOfThreads);
        MatrixProduct matrixProduct = new MatrixProduct(numberOfLinesA,numberOfColumnsALinesB,numberOfColumnsB,maxNumberOfElementsPerThread);
       
        double start = System.currentTimeMillis();

        for (int i=0; i<numberOfThreads; i++)
        {
            if(theWay == POOL_WAY)
                pool.execute(matrixProduct);
            else if(theWay == ASYNC_WAY)
                CompletableFuture.runAsync(matrixProduct::run).thenRunAsync(()-> {
                    numberOfThreadsDone ++;
                    if(numberOfThreads == numberOfThreadsDone){
                        completed = true;
                    }
                });
        }
        if(theWay == ASYNC_WAY) {
            while (!completed){}
        }
        if(theWay == POOL_WAY)
            pool.shutdown();

        double finish = System.currentTimeMillis();

        return finish - start;
    }

    private void statisticAddition(int numberOfLinesHere, int numberOfColumnsHere, int numberOfThreads){
        double nrAsync = 0;
        double nrPool = 0;
        int iterationNumber = 20;
        for(int i = 0; i < iterationNumber; ++i){
            nrAsync += mainAddition(numberOfLinesHere, numberOfColumnsHere, numberOfThreads, ASYNC_WAY);
            nrPool += mainAddition(numberOfLinesHere, numberOfColumnsHere, numberOfThreads, POOL_WAY);
        }
        printStatistic(numberOfThreads, nrAsync, nrPool, iterationNumber);
    }

    private void statisticMultiplication(int numberOfLinesA, int numberOfColumnsALinesB, int numberOfColumnsB, int numberOfThreads){
        double nrAsync = 0;
        double nrPool = 0;
        int iterationNumber = 5;
        for(int i = 0; i < iterationNumber; ++i){
            nrAsync += mainMultiplication(numberOfLinesA, numberOfColumnsALinesB, numberOfColumnsB, numberOfThreads, ASYNC_WAY);
            nrPool += mainMultiplication(numberOfLinesA, numberOfColumnsALinesB, numberOfColumnsB, numberOfThreads, POOL_WAY);
        }
        printStatistic(numberOfThreads, nrAsync, nrPool, iterationNumber);
    }


    private void printStatistic(int numberOfThreads, double nrAsync, double nrPool, int iterationNumber) {
        
        String divider = "-------------------------------";
        String async  = "Async   | "+ numberOfThreads +"  |  " + nrAsync / iterationNumber;
        String pool   = "Pool    | "+ numberOfThreads +"  |  " + nrPool / iterationNumber;
        System.out.println(divider);
        System.out.println(async);
        System.out.println(pool);
    }

    
    void run() {
        System.out.println("Addition");
       statisticAddition(1000, 1000, 1);
       statisticAddition(1000, 1000, 2);
       statisticAddition(1000, 1000, 4);
       statisticAddition(1000, 1000, 8);
        System.out.println("Multiplication");
        statisticMultiplication(200, 200, 200, 1);
        statisticMultiplication(200, 200, 200, 2);
        statisticMultiplication(200, 200, 200, 4);
        statisticMultiplication(200, 200, 200, 8);
    }
}
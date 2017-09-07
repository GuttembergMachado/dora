//
// Created by gutto on 06/09/17.
//
#include "transforms.h"

//========================================================================//
// A trapezoid that represents an document on a photo have the            //
// following notation:                                                    //
//                                                                        //
//              (A)------(B)                                              //
//              /          \                                              //
//             /      +     \                                             //
//            /              \                                            //
//           /                \                                           //
//         (D)----------------(C)                                         //
//                                                                        //
//========================================================================//

//vector<Point>    : MatOfPoint
//vector<Point2f>  : MatOfPoint2f
//vector<Point3i>  : MatOfPoint3
//vector<Point3f>  : MatOfPoint3f
//vector<KeyPoint> : MatOfKeyPoint
//vector<DMatch>   : MatOfDMatch
//vector<Rect>     : MatOfRect
//vector<uchar>    : MatOfByte
//vector<char>     : MatOfByte
//vector<int>      : MatOfInt
//vector<float>    : MatOfFloat
//vector<double>   : MatOfDouble
//vector<Vec4i>    : MatOfInt4
//vector<Vec4f>    : MatOfFloat4
//vector<Vec6f>    : MatOfFloat6


//Calculate a angle from 3 points
double angleFromPoints(Point pt1, Point pt2, Point pt0){
    
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    
    return ( (dx1 * dx2) + (dy1 * dy2)) / sqrt( ( (dx1 * dx1) + (dy1 * dy1)) * ( (dx2 * dx2) + (dy2 * dy2) ) + 1e-10);
}

//Reoders the 4 vertices of a trapezoid to use the above notation
bool reSortCorners(Point &A, Point &B, Point &C, Point &D){
    
    vector<Point> corners;
    vector<Point> topPoints;
    vector<Point> bottomPoints;
    
    try{
        Log(log_Debug, "transforms.cpp", "reSortCorners", "      Sorting corners...");
        
        Point aa = A;
        Point bb = B;
        Point cc = C;
        Point dd = D;
        
        corners.push_back(A);
        corners.push_back(B);
        corners.push_back(C);
        corners.push_back(D);
        
        //Ordena os pontos de acordo com a altura.
        //Collections.sort(Corners,new Comparator<Point>() {
        //    int compare(Point o1, Point o2) {
        //        return (int) o1.y -  (int) o2.y;
        //    }
        //});
        //
        //Os dois mais altos serão os novos A ou B
        //  topPoints.push_back(corners.get(0));
        //  topPoints.push_back(corners.get(1));
        //
        //Os dois mais baixos serão os novos C ou D
        //  bottomPoints.add(corners.get(2));
        //  bottomPoints.add(corners.get(3));
        //
        //Define quem é quem, baseado agora nos pontos mais a esquerda ou mais a direita
        //  A = (topPoints.get(0).x    > topPoints.get(1).x    ? topPoints.get(1)    : topPoints.get(0));
        //  B = (topPoints.get(0).x    > topPoints.get(1).x    ? topPoints.get(0)    : topPoints.get(1));
        //  C = (bottomPoints.get(0).x > bottomPoints.get(1).x ? bottomPoints.get(0) : bottomPoints.get(1));
        //  D = (bottomPoints.get(0).x > bottomPoints.get(1).x ? bottomPoints.get(1) : bottomPoints.get(0));
        
        Log(log_Detail, "transforms.cpp", "reSortCorners", "      Corners were sorted from 'A:(%i x %i), B:(%i x %i), C:(%i x %i) D:(%i x %i)' to 'A:(%i x %i), B:(%i x %i), C:(%i x %i) D:(%i x %i)'.",aa.x, aa.y, bb.x, bb.y, cc.x, cc.y, dd.x, dd.y, A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);
        return true;
        
    }catch (const std::exception &e) {
        Log(log_Error, "transforms.cpp", "reSortCorners", "Exception: %s",  e.what());
    }
    
    return false;
}

//Transforms the trapezoid part of a image into a rectangle
void deskewMat(Mat &inputMat, Mat &outputMat, Point A, Point B, Point C, Point D){
    
    int 	destWidth   = 0;
    int 	destHeight  = 0;
    
    try{
        Log(log_Detail, "transforms.cpp", "deskewMat", "      Deskewing mat...");
        
        //Sorts the corners of the trapezoid
        reSortCorners(A, B, C, D);
    
        Log(log_Detail, "transforms.cpp", "deskewMat", "         Trapezoid is 'A:(%i x %i), B:(%i x %i), C:(%i x %i) D:(%i x %i)'...", A.x, A.y, B.x, B.y, C.x, C.y, D.x, D.y);
    
        //Calculates the size of each line that composes the trapexoid
        auto AB = sqrt( pow(A.x - B.x , 2) + pow(A.y - B.y, 2)  );
        auto BC = sqrt( pow(B.x - C.x , 2) + pow(B.y - C.y, 2)  );
        auto CD = sqrt( pow(C.x - D.x , 2) + pow(C.y - D.y, 2)  );
        auto DA = sqrt( pow(D.x - A.x , 2) + pow(D.y - A.y, 2)  );
        
        Log(log_Detail, "transforms.cpp", "deskewMat", "         Trapezoid is %s: AB(%i), BC(%i), CD(%i) and DA(%i).", ((AB + CD) > (BC + DA) ? "HORIZONTAL": "VERTICAL"), AB, BC, CD, DA);

        //Define uma matriz com os pontos encontrados
        vector<Point> source;
        source.push_back(A);
        source.push_back(B);
        source.push_back(C);
        source.push_back(D);
        Mat sourceVertices = Converters.vector_Point2f_to_Mat(source);
        
        //Método 1: Calcula o maior retangulo capaz de conter a o trapézio--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        MatOfPoint2f v = new MatOfPoint2f(aa, bb, cc, dd);
        RotatedRect b = Imgproc.minAreaRect(v);
        destWidth  = (int) b.boundingRect().width;
        destHeight = (int) b.boundingRect().height;
        Log(log_Detail, "transforms.cpp", "deskewMat", "            Method 1 (min Rect) calculated: %i x %i.", destWidth, destHeight);
        
        //Método 2: Usa a menor de cada linha oposta encontrada-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        destWidth  = (AB < CD ? AB : CD);
        destHeight = (BC < DA ? BC : DA);
        Log(log_Detail, "transforms.cpp", "deskewMat", "            Method 2 (min Lenght) calculated: %i x %i.", destWidth, destHeight);
        
        //Método 3: Usa as médias das distancias----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        destWidth  = (AB+CD)/2;
        destHeight =  (BC+DA) /2;
        Log(log_Detail, "transforms.cpp", "deskewMat", "            Method 3 (Average Lenght) calculated: %i x %i.", destWidth, destHeight);
        
        //Usar o método 3
        destWidth  = ((AB+CD)/2)  ;
        destHeight =  ((BC+DA) /2) ;
        Log(log_Detail, "transforms.cpp", "deskewMat", "            Using Method 3.");
        
        //Define uma matriz com os pontos destino
        vector<Point> dest;
        dest.push_back( new Point(0,0));
        dest.push_back( new Point(destWidth,0));
        dest.push_back( new Point(destWidth,destHeight));
        dest.push_back( new Point(0,destHeight));
        Mat destVertices = Converters.vector_Point2f_to_Mat(dest);
        
        //Cria a matriz de relação entre os vértices de origem e de destino
        Mat relationMatrix  = Imgproc.getPerspectiveTransform(sourceVertices,destVertices);
        
        //Cria a Matrix destino
        Mat deskewedMatrix = new Mat(destWidth,destHeight, CV_8U);
        
        //Usa a matriz de relação para de fato fazer o deskew
        Imgproc.warpPerspective(inputMat, deskewedMatrix, relationMatrix, new Size(destWidth, destHeight) );
    
        Log(log_Detail, "transforms.cpp", "reSortCorners", "            Finshed deskewing.");
        return true;
    
    }catch (const std::exception &e) {
        Log(log_Error, "transforms.cpp", "reSortCorners", "Exception: %s",  e.what());
    }
    
    return false;;
}

//Detects a border (probably a trapezoid like form)representing a document on a picture
void detectBorder(Mat &inputMat, Mat &outputMat){
    
    try{
    
        Log(log_Debug, "transforms.cpp", "detectBorder", "borderDetectionProc",   "Starting border detection process...");
        //--------------------------------------------------------------------------------------------------------------------------------------
        
        vector<vector<Point>> 	contours;
        Mat 				    singlecolorMat;
        Mat 				    binaryMat;
        int 				    UPPER_TRESHOLD  = 8;
        vector<Point>		    p;
        
        //Cria uma cópia zerada do Mat
        Utils.bitmapToMat(scaledBitmapReadOnly, inputMat);
    
        Log(log_Debug, "transforms.cpp", "detectBorder", "borderDetectionProc",   "   Image is %i x %i." + inputMat.rows() + inputMat.cols() + "..." );
        
        //1) Borra a imagem para remover noise (no código original é usado um downscale/upscale com as funções pyrDown/PyrUp)
        Imgproc.medianBlur(workMat, workMat,5);
        
        //Zera o contador de área( DEBUG ONLY)
        double biggestArea = 0;
        double otsu_threshold = 0;
        
        //2) Processa cada uma dos 3 paineis de cores (RGB) separadamente
        for(int c =0; c < 3; c++ ){
    
            Log(log_Debug, "transforms.cpp", "detectBorder", "      borderDetectionProc", "   Trying channel %s...", c);
            
            //3) Extrai a cor em questão (no código original é usado outras funções).
            Core.extractChannel(workMat, singlecolorMat, c);
            
            //4) Tenta diferentes combinações de treshold
            for(int t = -2 ; t < UPPER_TRESHOLD; t++ ){
                
                //5)Decide qual método usar para separar as bordas
                switch(t){
                    case -2:
                        Log(log_Debug, "transforms.cpp", "detectBorder", "      Trying Threshold with THRESH_OTSU...");
                        otsu_threshold = Imgproc.threshold(singlecolorMat, binaryMat,0 , 255, Imgproc.THRESH_BINARY + Imgproc.THRESH_OTSU );
                        break;
                    
                    case -1:
                        Log(log_Debug, "transforms.cpp", "detectBorder","      Trying AdaptiveThreshold...");
                        Imgproc.adaptiveThreshold(singlecolorMat, binaryMat, 255, Imgproc.ADAPTIVE_THRESH_MEAN_C, Imgproc.THRESH_BINARY_INV, 11, 2);
                        break;
                    
                    case 0:
                        Log(log_Debug, "transforms.cpp", "detectBorder","      Trying Canny/Dilate...");
                        Imgproc.Canny(singlecolorMat, binaryMat, otsu_threshold * .5 , otsu_threshold);
                        Imgproc.dilate(binaryMat, binaryMat, Imgproc.getStructuringElement(Imgproc.MORPH_ELLIPSE, new Size(5,5)),new Point(-1,-1),1);
                        break;
                    
                    default:
                        Log(log_Debug, "transforms.cpp", "detectBorder","      Trying Treshold " + (t + 1) + "...");
                        Imgproc.threshold(singlecolorMat, binaryMat, t * 255 / (UPPER_TRESHOLD -3) , 255, Imgproc.THRESH_BINARY );
                        break;
                }
                
                
                //6) Detecta os contornos presentes na imagem (código original era RETR_LIST)
                contours.clear();
                Imgproc.findContours(binaryMat, contours, new Mat(), Imgproc.RETR_EXTERNAL , Imgproc.CHAIN_APPROX_SIMPLE);
                Log(log_Debug, "transforms.cpp", "detectBorder", "        Found %i contours. Searching for quadrilaterals among them...",  contours.size());
                
                //7) Testa os contornos encontrados
                for (int i = 0; i < contours.size(); i++)
                {
                    MatOfPoint polygon = contours.get(i);
                    
                    MatOfPoint2f mMOP2f1 = new MatOfPoint2f();
                    MatOfPoint2f mMOP2f2 = new MatOfPoint2f();
                    
                    //Converte o contorno em  MatOfPoint2f
                    polygon.convertTo(mMOP2f1, CV_32FC2);
                    
                    //Tenta localizar polígonos usando os contornos encontrados
                    Imgproc.approxPolyDP(mMOP2f1, mMOP2f2, Imgproc.arcLength(mMOP2f1, true) * 0.1, true);
                    
                    //Convert o MatOfPoint2f de volta para contorno
                    mMOP2f2.convertTo(polygon, CV_32S);
                    
                    //Verifica se o polígono encontrado tem 4 lados
                    if (polygon.total() == 4)
                    {
                        //Verifica se o polígono é convexo
                        if (Imgproc.isContourConvex(polygon)){
                            
                            //Obtem os pontos desse polígono (vértices)
                            p = polygon.toArray();
                            
                            //Calcula a área do polígono
                            double Area = Math.abs(Imgproc.contourArea(polygon));
                            
                            //Verifica se a área do polígono é grande o suficiente (mais de 5000 pixels) e menor do que 95% da imagem
                            if (Area >= 5000 && Area <  (workMat.rows() * workMat.cols() * 0.95)) {
    
                                Log(log_Debug, "transforms.cpp", "detectBorder",            "Polygon " + i + "(channel=" + c + ",treshold=" + t + ") has an area of " + Area + "(A=" + p[0].toString() + ", B=" + p[1].toString() + ", C=" + p[2].toString() + ", D=" + p[3].toString() + ")...");
                                
                                //Verifica se é a maior área até o momento
                                if (Area > biggestArea){
                                    
                                    //Verifica se os angulos desse polígono indicam que ele é algo parecido com um quadrado/retangulo/trapezio/paralelogramo/etc
                                    double maxCosine = 0;
                                    
                                    for (int j = 2; j < 5; j++)
                                    {
                                        double cosine = Math.abs(angle_frompoints(p[j%4], p[j-2], p[j-1]));
                                        maxCosine = Math.max(maxCosine, cosine);
                                    }
                                    
                                    // if cosines of all angles are small  (all angles are ~90 degree) then write quandrang  vertices to resultant sequence
                                    if (maxCosine < 0.3){
                                        
                                        //Anota esses pontos (convertendo os valores para inteiro)
                                        A = new Point( (int) p[0].x, (int)  p[0].y);
                                        B = new Point( (int) p[1].x, (int)  p[1].y);
                                        C = new Point( (int) p[2].x, (int)  p[2].y);
                                        D = new Point( (int) p[3].x, (int)  p[3].y);
                                        
                                        //Anota a maior área encontrada até agora
                                        biggestArea = Area;
    
                                        Log(log_Debug, "transforms.cpp", "detectBorder",  "            The biggest quadrilateral so far is polygon " + i + "(cor=" + c + ",treshold=" + t + "). It's area is  " + Area + "(A=" + A.toString() + ", B=" + B.toString() + ", C=" + C.toString() + ", D=" + D.toString() + ")...");
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        //Se não encontrou, usa o padrão
        if (biggestArea == 0){
            Log(log_Debug, "transforms.cpp", "detectBorder",   "   Unable to find a border.");
        }else{
            detectedPoints = "A(" + (int) A.x + ", " + (int) A.y +  ") B(" + (int) B.x +  ", " + (int) B.y +  ") C(" + (int)  C.x +  ", " + (int) C.y +  ") D(" + (int) D.x +  ", " + (int) D.y +  ")";
            Log(log_Debug, "transforms.cpp", "detectBorder",   "   The border was detected at " + detectedPoints);
        }
    
        Log(log_Debug, "transforms.cpp", "detectBorder",  "Finished detecting borders.");
        return true;
    
    }catch (const std::exception &e) {
        Log(log_Error, "transforms.cpp", "detectBorder", "Exception: %s",  e.what());
    }
}


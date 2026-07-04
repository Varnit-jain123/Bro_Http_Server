#include<bro.h>
// Bobby [The Web application developer - User o<f bro web server]
class Bulb:public Stringifyable
{
    private:
        int wattage;
        int price;
    public:
        void setWattage(int wattage)
        {
            this->wattage=wattage;
        }
        int getWattage()
        {
            return this->wattage;
        }
        void setPrice(int price)
        {
            this->price=price;
        }
        int getPrice()
        {
            return this->price;
        }
        string stringify()
        {
            return "Bulb wattage is = "+to_string(this->wattage)+", price = "+to_string(this->price);
        }
};

int main()
{
    try
    {
        Bro bro;
        bro.setStaticResourceFolder("Whatever");
        bro.addStartupService(2,[](){
            cout<<"---------------------------------------------"<<endl;
            cout<<"some cool function that get call on startup"<<endl;
            cout<<"The priority number set for this function is 2"<<endl;
            cout<<"---------------------------------------------"<<endl;
        });
        bro.addStartupService(1,[](){
            cout<<"---------------------------------------------"<<endl;
            cout<<"some great function that get call on startup"<<endl;
            cout<<"The priority number set for this function is 1"<<endl;
            cout<<"---------------------------------------------"<<endl;
        });
        bro.addStartupService(1,[](){
            cout<<"---------------------------------------------"<<endl;
            cout<<"some great too function that get call on startup"<<endl;
            cout<<"The priority number set for this function is 1"<<endl;
            cout<<"---------------------------------------------"<<endl;
        });
        bro.addStartupService(3,[](ApplicationLevelContainer &applicationLevelContainer){
            cout<<"---------------------------------------------"<<endl;
            cout<<"some too cool function that get call on startup"<<endl;
            cout<<"The priority number set for this function is 3"<<endl;
            cout<<"---------------------------------------------"<<endl;
        });

        // bro.get("/getCustomers", [](Request &request, Response &response) {
        //     const char *html = R""""(
        //     <!DOCTYPE html>
        //     <html>
        //     <body>
        //         <h1>Welcome Administrator</h1>
        //         <a href="getCustomers">Customers list</a>
        //     </body>
        //     </html>
        //     )"""";
        //     response.setContentType("text/html");
        //     response << html;
        // });

        // bro.get("/getCustomers", [](Request &request, Response &response) {
        //     const char *html = R""""(
        //     <!DOCTYPE html>
        //     <html>
        //     <body>
        //         <h1>List of Customers</h1>
        //         <ul>
        //             <li>Amit</li>
        //             <li>Bobby</li>
        //         </ul>
        //         <br>
        //         <a href="/">Home</a>
        //     </body>
        //     </html>
        //     )"""";
        //     response.setContentType("text/html");
        //     response << html;
        // }); 


        bro.get("/save_test1_data", [](Request &request, Response &response) {
            string nnn=request["nm"];
            string ccc=request["ct"];
            cout<<"Data that arrived in request"<<endl;
            cout<<nnn<<endl;
            cout<<ccc<<endl;
            cout<<"------------------------"<<endl;
            const char *html = R""""(
            <!DOCTYPE html>
            <html>
            <head>
            <meta charset='utf-8'>
            <title>Bro test cases</title>
            </head>
            <body>
                <h1>Test case 1 - GET with query string</h1>
                <h3>Response from server side</h3>
                <b>Data Saved</b>
                <br/><br/>
                <a href="/index.html">Home</a>
            </body>
            </html>
            )"""";
            response.setContentType("text/html");
            response << html;
        });

        bro.post("/save_test2_data", [](Request &request, Response &response) {
            string nm = request["nm"];
            string ct = request["ct"];
            string gender = request["gender"];

            cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
            cout<<"Form Data Received:"<<endl;
            cout<<"Name: "<<nm<<endl;
            cout<<"City Code: "<<ct<<endl;
            cout<<"Gender: "<<gender<<endl;
            cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;

            Cookie c1("Name", nm);
            Cookie c2("CityCode", ct);
            Cookie c3("Gender", gender);
            response.addCookie(c1);
            response.addCookie(c2);
            response.addCookie(c3);
            const char *html = R""""(
            <!DOCTYPE html>
            <html>
            <head>
            <meta charset='utf-8'>
            <title>Bro test cases</title>
            </head>
            <body>
                <h1>Test case 2 - POST with form data</h1>
                <h3>Response from server side</h3>
                <b>Data Saved</b>
                <br/><br/>
                <a href="/index.html>Home</a>
            </body>
            </html>
            )"""";
            response.setContentType("text/html");
            response << html;
        });
            
        bro.get("/firstCartoonFilm", [](Request &request, Response &response,ApplicationLevelContainer &cc) {
            string *str;
            str=new string("The jungle book");
            cc.set("firstFilm",str,NULL,NULL);
            const char *html = R""""(
            <!DOCTYPE html>
            <html>
            <head>
            <meta charset='utf-8'>
            <title>Bro test cases</title>
            </head>
            <body>
                <h1>First cartoon film</h1>
                <h3>The jungle book</h3>
                <br><br>
                <a href="/secondCartoonFilm">Watch the next film</a>
            </body>
            </html>
            )"""";
            response.setContentType("text/html");
            response << html;
        });

        bro.get("/secondCartoonFilm", [](Request &request, Response &response,ApplicationLevelContainer &cc) {
            string *s;
            cc.get("firstFilm",&s,NULL,NULL);
            response.setContentType("text/html");
            const char *html1 = R""""(
                <!DOCTYPE html>
                <html>
                <head>
                <meta charset='utf-8'>
                <title>Bro test cases</title>
                </head>
                <body>
                <h1>First film was 
                )"""";
            response<<html1;
            response<<*s;
            const char *html2=R""""(
                    </h1>
                    <br><br>
                    <h1>Second cartoon film</h1>
                    <h3>The Toy Story</h3>
                    <br><br>
                    <br/><br/>
                </body>
                </html>
                )"""";
            response<<html2;
        });
        
        bro.get("/coolOne", [](Request &request, Response &response) {
            cout<<"some processing is done over here by function mapped to /coolOne"<<endl;
            _forward_(request,string("/coolTwo"));
        });

        bro.get("/coolTwo", [](Request &request, Response &response) {
            cout<<"some processing is done over here by function mapped to /coolTwo"<<endl;
            _forward_(request,string("/coolThree"));
            cout<<"Some lines that should not executed as request been forwarded"<<endl;
        });

        bro.get("/coolThree", [](Request &request, Response &response) {
            cout<<"some processing is done over here by function mapped to /coolThree"<<endl;
            _forward_(request,string("/somethingCool.html"));
        }); 

        /*
            Bobby wants to specify the function(s) that shoud be called ,
            when server starts
            the function may require (ApplicationLevelContainer)
            
            Bobby should also be able to specify the priority level of the startup function
            priority will be decided the sequence of execution of the function specified by bobby
            weighted priority level 
            1 a
            2 b
            3 c
        */

        bro.get("/Slogan", [](Request &request, Response &response) {
            string slogan;
            ifstream iFile("data/sofd.data");
            string line;
            while(true)
            {
                if(!getline(iFile,line)) break;
                if(slogan.length()>0) slogan+=string("<br>");
                slogan+=line;
            }
            iFile.close();
            response.setContentType("text/html");
            const char *html = R""""(
            <!DOCTYPE html>
            <html>
            <head>
            <meta charset='utf-8'>
            <title>Bro test cases</title>
            </head>
            <body>
                <h1>Words of Wisdom</h1>
            )"""";
            response<<html;
            response<<slogan.c_str();
            const char*html2=R""""(
            </body>
            </html>
            )"""";
            response<<html2;
            
        });

        bro.get("/SloganOfTheDay", [](Request &request, Response &response) {
            string slogan;
            ifstream iFile("data/sofd.data");
            string line;
            while(true)
            {
                if(!getline(iFile,line)) break;
                if(slogan.length()>0) slogan+=string("<br>");
                slogan+=line;
            }
            iFile.close();
            cout<<"size of slogan of the day is : "<<slogan.length()<<endl;
            request.setCHTMLVariable("sloganOfTheDay",slogan);
            request.setCHTMLVariable("city1","indore");
            request.setCHTMLVariable("city2","goa");
            Bulb bulb;
            bulb.setWattage(100);
            bulb.setPrice(250);
            request.setCHTMLVariable("bulb",&bulb);
            short int a=10;
            unsigned short int b=65520;
            int c=-200000;
            unsigned int d=4000000000;  
            long int e=-9000000000;
            unsigned long int f=18000000000;
            float g=3.14159f;
            double h=2.718281828459045;
            bool i=true;
            request.setCHTMLVariable("a",a);
            request.setCHTMLVariable("b",b);
            request.setCHTMLVariable("c",c);
            request.setCHTMLVariable("d",d);
            request.setCHTMLVariable("e",e);
            request.setCHTMLVariable("f",f); 
            request.setCHTMLVariable("g",g);
            request.setCHTMLVariable("h",h);
            request.setCHTMLVariable("i",i);
            _forward_(request,string("/WordsOfWisdom.chtml"));   
        });

        bro.get("/aRequest",[](Request &request, Response &response) {
            cout<<"some processing is done over here by function mapped to /aRequest"<<endl;
            // 100 as score is generated as per application reqeuirement
            int x=100;
            request.set("score",x,NULL,NULL);
            cout<<"Now forwarding the request to /bRequest "<<endl;
            _forward_(request,string("/bRequest"));
        });

        bro.get("/bRequest",[](Request &request, Response &response) {
            cout<<"some processing is done over here by function mapped to /bRequest"<<endl;
            // 100 as score is generated as per application reqeuirement
            int whatever;
            request.get("score",&whatever,NULL,NULL);
            cout<<"Score received from previous request handler is : "<<whatever<<endl;
            Cookie c1("RollNumber","101");
            Cookie c2("Name","Bobby");
            Cookie c3("City","Indore");
            response<<c1<<c2;
            response.addCookie(c3);
            response.setContentType("text/html");
            response<<"<html><head></head><body>Cool</body></html>";
        });

        bro.listen(6060, [](Error &error) {
            if (!error.hasError())
                cout << "Bro HTTP Server is listening on port 6060\n";
            else
                cout << "Error occurred: " << error.getError() << endl;
        });
    }catch(string exception)
    {
        cout<<exception<<endl;
    }
    return 0;
}
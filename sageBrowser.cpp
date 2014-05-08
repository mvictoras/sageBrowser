#include <iostream>
#include <Awesomium/WebCore.h>
#include <Awesomium/BitmapSurface.h>
#include <Awesomium/STLHelpers.h>

#include <libsage.h>

using namespace Awesomium;

class Browser {
public:
    WebView* mView;
    WebCore* mWebCore;

    std::string mUrl;
    size_t mWidth;
    size_t mHeight;


public:

    Browser(std::string url, size_t width, size_t height ) :
    mUrl(url), mWidth(width), mHeight(height) {
        mWebCore = WebCore::Initialize(WebConfig());
        mView = mWebCore->CreateWebView(width, height, 0, kWebViewType_Offscreen);

        // XXXXX
        WebURL webUrl(WSLit(url.c_str()));
        mView->LoadURL(webUrl);

        while (mView->IsLoading())
           mWebCore->Update();

    }


    const unsigned char* getScreenshot() {
        mWebCore->Update();
        BitmapSurface* surface = (BitmapSurface*)mView->surface();

        if (surface != 0) {
            return surface->buffer();
        } else {
            std::cout << "Error in getScreenshot. Surface is NULL." << std::endl;
        }

        return NULL;
    }

    void resize(int width, int height) {
        mWidth = width;
        mHeight = height;
        mView->Resize(width, height);
    }

    void click(int x, int y) {
        mView->InjectMouseMove(x, y);
        mView->InjectMouseDown(kMouseButton_Left);
        mView->InjectMouseUp(kMouseButton_Left);
    }
};

int main(int argc, char** argv) {
    sail *sageInf;
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    float ratio = (float) height / (float) width;
    if(width > 1920 * 6 ) 
        width = 1920 * 6;
        height = (int) (ratio * width);

    if(height > 1080 * 3)
        height = 1080 * 3;
        width = (int) (ratio * (float) height); 


    sageInf = createSAIL("sageBrowser", width, height, PIXFMT_8888_INV, NULL, TOP_TO_BOTTOM);

    Browser browser(argv[1], width, height);

    bool quit = false;
    while(!quit) {
        const unsigned char* texture = browser.getScreenshot();
        if( texture ) {
            swapWithBuffer(sageInf, (unsigned char*)texture);
        }

        int id, a, down;
        float x, y, scroll;
        sageMessage msg;
        if (sageInf->checkMsg(msg, false) > 0) {
            char *data = (char *) msg.getData();
            switch (msg.getCode()) {
                case APP_QUIT:
                    sageInf->shutdown();
                    quit = true;
                    break;
                case EVT_MOVE:
		case EVT_PAN:
                    sscanf(data, "%d %f %f", &id, &x, &y);
                    browser.mView->InjectMouseMove(x * width, height - y * height);
                    break;
                case EVT_CLICK:
                    sscanf(data, "%d %f %f %d %d", &id, &x, &y, &a, &down);
                    if( down == 1 )
		        browser.mView->InjectMouseDown(kMouseButton_Left);
                    else
		        browser.mView->InjectMouseUp(kMouseButton_Left);
                    break;
		case EVT_ZOOM:
		    sscanf(data, "%d %f %f %f", &id, &x, &y, &scroll);
		    browser.mView->InjectMouseWheel(0, (int)(scroll * 100));
		    break;
            }
        }
    }

    //deleteSAIL(sageInf);

    return 0;
}

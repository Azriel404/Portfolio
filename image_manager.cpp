#include "image_manager.h"

/*
    private
*/

/*==== 画像ファイルをビットマップ化する ====*/
ID2D1Bitmap* ImageManager::LoadBitmapFromFile(const wchar_t* filename) {

    /*-- ビットマップ化に必要な装置を宣言 --*/
    IWICImagingFactory* pWICFactory = nullptr;
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pFrame = nullptr;
    IWICFormatConverter* pConverter = nullptr;


    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        reinterpret_cast<void**>(&pWICFactory)
    );

    if (SUCCEEDED(hr))
    {
        hr = pWICFactory->CreateDecoderFromFilename(
            filename,
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &pDecoder
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pFrame);
    }

    if (SUCCEEDED(hr))
    {
        hr = pWICFactory->CreateFormatConverter(&pConverter);
    }

    if (SUCCEEDED(hr))
    {
        hr = pConverter->Initialize(
            pFrame,
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0,
            WICBitmapPaletteTypeMedianCut
        );
    }

    ID2D1Bitmap* pBitmap = nullptr;
    if (SUCCEEDED(hr))
    {
        hr = pRenderTarget->CreateBitmapFromWicBitmap(
            pConverter,
            nullptr,
            &pBitmap
        );
    }

    pWICFactory->Release();     pWICFactory = nullptr;
    pDecoder->Release();        pDecoder = nullptr;         // ここでエラーが出たら、画像ファイルが見つかっていない
    pFrame->Release();          pFrame = nullptr;
    pConverter->Release();      pConverter = nullptr;

    return pBitmap;


}




/*
    public
*/

ImageManager::ImageManager(ID2D1HwndRenderTarget* pRenderTarget) :
    pRenderTarget(pRenderTarget)
{

}

ImageManager::~ImageManager() {

}


// 画像ファイルをビットマップに変換する
ID2D1Bitmap* ImageManager::TransferFromFile(const wchar_t* filename) {

    /*-- ビットマップ化に必要な装置を宣言 --*/
    IWICImagingFactory* pWICFactory = nullptr;
    IWICBitmapDecoder* pDecoder = nullptr;
    IWICBitmapFrameDecode* pFrame = nullptr;
    IWICFormatConverter* pConverter = nullptr;


    HRESULT hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        reinterpret_cast<void**>(&pWICFactory)
    );

    if (SUCCEEDED(hr))
    {
        hr = pWICFactory->CreateDecoderFromFilename(
            filename,
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &pDecoder
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pFrame);
    }

    if (SUCCEEDED(hr))
    {
        hr = pWICFactory->CreateFormatConverter(&pConverter);
    }

    if (SUCCEEDED(hr))
    {
        hr = pConverter->Initialize(
            pFrame,
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0,
            WICBitmapPaletteTypeMedianCut
        );
    }

    ID2D1Bitmap* pBitmap = nullptr;
    if (SUCCEEDED(hr))
    {
        hr = pRenderTarget->CreateBitmapFromWicBitmap(
            pConverter,
            nullptr,
            &pBitmap
        );
    }

    pWICFactory->Release();     pWICFactory = nullptr;
    pDecoder->Release();        pDecoder = nullptr;         // ここでエラーが出たら、画像ファイルが見つかっていない
    pFrame->Release();          pFrame = nullptr;
    pConverter->Release();      pConverter = nullptr;

    return pBitmap;

}


void ImageManager::Add(const wchar_t* filename, float targetWidth, float targetHeight, float x, float y) {

    ID2D1Bitmap* pBitmap = LoadBitmapFromFile(filename);
    if (pBitmap)
    {
        images.push_back({ pBitmap, targetWidth, targetHeight, x, y });
    }

}


void ImageManager::Draw() {

    for (auto& img : images) {

        pRenderTarget->DrawBitmap(
            img.pBitmap,
            D2D1::RectF(
                img.x,
                img.y,
                img.x + img.targetWidth,
                img.y + img.targetHeight
            ),
            1.0f,  // 不透明度
            D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,  // 縮小時の補間モード
            D2D1::RectF(
                0.0f,  // 描画元の左上のX座標
                0.0f,  // 描画元の左上のY座標
                img.pBitmap->GetSize().width,  // 描画元の右下のX座標
                img.pBitmap->GetSize().height  // 描画元の右下のY座標
            )
        );
    }
}

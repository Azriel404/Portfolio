#include "image_manager.h"

/*
    private
*/

/*==== �摜�t�@�C�����r�b�g�}�b�v������ ====*/
ID2D1Bitmap* ImageManager::LoadBitmapFromFile(const wchar_t* filename) {

    /*-- �r�b�g�}�b�v���ɕK�v�ȑ��u��錾 --*/
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
    pDecoder->Release();        pDecoder = nullptr;         // �����ŃG���[���o����A�摜�t�@�C�����������Ă��Ȃ�
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


// �摜�t�@�C�����r�b�g�}�b�v�ɕϊ�����
ID2D1Bitmap* ImageManager::TransferFromFile(const wchar_t* filename) {

    /*-- �r�b�g�}�b�v���ɕK�v�ȑ��u��錾 --*/
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
    pDecoder->Release();        pDecoder = nullptr;         // �����ŃG���[���o����A�摜�t�@�C�����������Ă��Ȃ�
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
            1.0f,  // �s�����x
            D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,  // �k�����̕�ԃ��[�h
            D2D1::RectF(
                0.0f,  // �`�挳�̍����X���W
                0.0f,  // �`�挳�̍����Y���W
                img.pBitmap->GetSize().width,  // �`�挳�̉E����X���W
                img.pBitmap->GetSize().height  // �`�挳�̉E����Y���W
            )
        );
    }
}

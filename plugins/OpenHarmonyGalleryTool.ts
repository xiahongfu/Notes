import hilog from '@ohos.hilog';

export function RegisterOpenHarmonyGalleryTool()
{
	hilog.info(0x000, 'testTag', 'RegisterOpenHarmonyGalleryTool!');
	const register = {};
	register["OpenHarmonyGalleryTool"] =  new OpenHarmonyGalleryTool();
	return register;
}

export class OpenHarmonyGalleryTool
{
	public saveImageToGallery(path : string)
	{
		globalThis.workerPort.postMessage({type: 'RUN_ON_UI_THREAD_JS', funcName: 'saveImageToGallery', imgPath: path});
	}
}
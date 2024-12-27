import photoAccessHelper from '@ohos.file.photoAccessHelper';
import hilog from '@ohos.hilog';
import { fileIo } from '@kit.CoreFileKit';
import { BusinessError } from '@kit.BasicServicesKit';

class Tags {
	static logTag: string = "[hm]"
}

export class OpenHarmonyGallery
{
	static async transferFileToGallery(srcFile: string, dstFile: string) {
		let stat = fileIo.statSync(srcFile);
		let file = fileIo.openSync(srcFile, fileIo.OpenMode.READ_WRITE);
		let buf = new ArrayBuffer(stat.size);
		fileIo.readSync(file.fd, buf);
		fileIo.closeSync(file);

		const fileTo = fileIo.openSync(dstFile, fileIo.OpenMode.READ_WRITE | fileIo.OpenMode.CREATE);
		fileIo.write(fileTo.fd, buf, (err: BusinessError, writeLen: number) => {
			if (err) {

			} else {

			}
			fileIo.closeSync(fileTo);
		});
	}

	static async saveImageToGallery(filename: string)  {
		let photoCreationConfigs: Array<photoAccessHelper.PhotoCreationConfig> = [
		  {
				fileNameExtension: 'png',
				photoType: photoAccessHelper.PhotoType.IMAGE,
				subtype: photoAccessHelper.PhotoSubtype.DEFAULT, // 可选
		  }
		];
		let srcFileUris: Array<string> = [
			filename
		];
		let context = globalThis.AbilityContext;
		let phAccessHelper = await photoAccessHelper.getPhotoAccessHelper(context);
		await phAccessHelper.showAssetsCreationDialog(srcFileUris, photoCreationConfigs)
		.then((uris: Array<string>) => {
			if (uris[0]) {
				console.log(Tags.logTag + "cp_" + uris[0])
				this.transferFileToGallery(filename, uris[0]);
			}else {
			}
		});
	}
}
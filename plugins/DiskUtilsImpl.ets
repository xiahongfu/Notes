import common from '@ohos.app.ability.common';
import statvfs from '@ohos.file.statvfs'; //�ļ�ϵͳ�ռ�ͳ��
import { GetFromGlobalThis } from './common/GlobalThisUtil'

export class DiskUtils {
  getTotalSizeSync(): number {
    let context: common.UIAbilityContext = GetFromGlobalThis("context") as common.UIAbilityContext;
    let number: number = statvfs.getTotalSizeSync(context.filesDir); // /data/storage/el2/base/haps/entry/files;
    return number / 1048576;
  }

	getFreeSizeSync(): number {
		let context: common.UIAbilityContext = GetFromGlobalThis("context") as common.UIAbilityContext;
    let number: number = statvfs.getFreeSizeSync(context.filesDir);
    return number / 1048576;
  }

	getBusySizeSync(): number {
		let context: common.UIAbilityContext = GetFromGlobalThis("context") as common.UIAbilityContext;
    let total: number = statvfs.getTotalSizeSync(context.filesDir);
    let free: number = statvfs.getFreeSizeSync(context.filesDir);
    return (total - free) / 1048576;
  }
}

export function RegisterDiskUtilsImpl() {
	let register: Record<string, object> = {}
  register["DiskUtils"] = new DiskUtils();
  return register;
}

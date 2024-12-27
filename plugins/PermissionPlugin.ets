import { abilityAccessCtrl, Context, common, bundleManager, Permissions } from '@kit.AbilityKit';
import { BusinessError } from '@kit.BasicServicesKit';
import { notificationManager } from '@kit.NotificationKit';
import { GetFromGlobalThis } from './common/GlobalThisUtil'

export class PermissionControl {
  reqPermissionOnSetting(permissionStr: string): void {
    // const permissions: Array<string> = ['ohos.permission.MICROPHONE', 'ohos.permission.SUBSCRIBE_NOTIFICATION_WINDOW_STATE'];
    let atManager: abilityAccessCtrl.AtManager = abilityAccessCtrl.createAtManager();
    let context: common.UIAbilityContext = GetFromGlobalThis("context") as common.UIAbilityContext;
    let permissions: Array<Permissions> = [permissionStr as Permissions];

    if (permissionStr == 'ohos.permission.LOCATION') {
      permissions.push('ohos.permission.APPROXIMATELY_LOCATION')
    }
    atManager.requestPermissionOnSetting(context, permissions).then((data: Array<abilityAccessCtrl.GrantStatus>) => {
      console.info('>>>>' + JSON.stringify(data));
    }).catch((err: BusinessError) => {
      console.error('>>>>' + JSON.stringify(err));
      if (err.code == 12100012) {
        // 输入的权限中存在未被用户拒绝过的权限
        this.reqPermissionsFromUser(permissions, context);
      }
    });
  }
  reqPermissionsFromUser(permissions: Array<Permissions>, context: common.UIAbilityContext): void {
    let atManager: abilityAccessCtrl.AtManager = abilityAccessCtrl.createAtManager();
    // requestPermissionsFromUser会判断权限的授权状态来决定是否唤起弹窗
    atManager.requestPermissionsFromUser(context, permissions).then((data) => {
      let grantStatus: Array<number> = data.authResults;
      let length: number = grantStatus.length;
      for (let i = 0; i < length; i++) {
        if (grantStatus[i] === 0) {
          // 用户授权，可以继续访问目标操作
        } else {
          // 用户拒绝授权，提示用户必须授权才能访问当前页面的功能，并引导用户到系统设置中打开相应的权限
          return;
        }
      }
      // 授权成功
    }).catch((err: BusinessError) => {
      console.error(`Failed to request permissions from user. Code is ${err.code}, message is ${err.message}`);
    })
  }
  checkSelfPermission(permissionStr: string): boolean{
    let permission: Permissions = permissionStr as Permissions;
    let grantStatus: abilityAccessCtrl.GrantStatus = this.checkPermissionGrant(permission);
    return grantStatus === abilityAccessCtrl.GrantStatus.PERMISSION_GRANTED;
  }
  checkSelfPermissionStatus(permissionStr:string): number{
    // 0 明确被拒绝    1 明确被允许      2 拒绝且不再询问       3 没有授权，需要第一次询问。
    // 鸿蒙暂不支持查询是否第一次询问。
    if (this.checkSelfPermission(permissionStr)) return 1;
    return 3;
  }
  checkPermissionGrant(permission: Permissions): abilityAccessCtrl.GrantStatus {
    let atManager: abilityAccessCtrl.AtManager = abilityAccessCtrl.createAtManager();
    let grantStatus: abilityAccessCtrl.GrantStatus = abilityAccessCtrl.GrantStatus.PERMISSION_DENIED;

    // 获取应用程序的accessTokenID
    let tokenId: number = 0;
    try {
      let bundleInfo: bundleManager.BundleInfo = bundleManager.getBundleInfoForSelfSync(bundleManager.BundleFlag.GET_BUNDLE_INFO_WITH_APPLICATION);
      let appInfo: bundleManager.ApplicationInfo = bundleInfo.appInfo;
      tokenId = appInfo.accessTokenId;
    } catch (error) {
      const err: BusinessError = error as BusinessError;
      console.error(`Failed to get bundle info for self. Code is ${err.code}, message is ${err.message}`);
    }

    // 校验应用是否被授予权限
    try {
      grantStatus = atManager.checkAccessTokenSync(tokenId, permission);
    } catch (error) {
      const err: BusinessError = error as BusinessError;
      console.error(`Failed to check access token. Code is ${err.code}, message is ${err.message}`);
    }

    return grantStatus;
  }

  areNotificationsEnabled():boolean {
    return notificationManager.isNotificationEnabledSync();
  }
  requestEnableNotification():void {
    let context: common.UIAbilityContext = GetFromGlobalThis("context") as common.UIAbilityContext;
    let isEnabled: boolean = notificationManager.isNotificationEnabledSync();
    if (isEnabled) return ;
    notificationManager.requestEnableNotification(context).then(() => {
    }).catch((err : BusinessError) => {
      if(1600004 == err.code){ // 用户允许

      } else {

      }
    });
  }
}


export function RegisterPermissionPlugin() {
  let register: Record<string, object> = {}
  register["PermissionPlugin"] = new PermissionControl();
  return register;
}
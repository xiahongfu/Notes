import { BusinessError } from '@kit.BasicServicesKit';
import { calendarManager } from '@kit.CalendarKit';
import { notificationManager } from '@kit.NotificationKit';
import {
  abilityAccessCtrl,
  common,
  PermissionRequestResult,
  Permissions,
  bundleManager
} from '@kit.AbilityKit';
import { GetFromGlobalThis } from './common/GlobalThisUtil'
import { data } from '@kit.TelephonyKit';

export class CalendarManager {
  // 指定日历账户信息
  private static calendarAccount: calendarManager.CalendarAccount = {
    name: 'MyCalendar',
    type: calendarManager.CalendarType.LOCAL,
    // 日历账户显示名称，该字段如果不填，创建的日历账户在界面显示为空字符串。
    displayName: '魂斗罗日程'
  };
  private static config: calendarManager.CalendarConfig = {
    enableReminder: true,
    color: '#aabbcc'
  }
  public static AddCalendar(title: string, content: string, remindTime: number, endTime: number): boolean {
    // UnityAndroidPermission 138
    //     let calendarMgr: calendarManager.CalendarManager | null = this.getCalendarMgr();
    let calendarMgr: calendarManager.CalendarManager | null = null;
    const permissions: Permissions[] = ['ohos.permission.READ_CALENDAR', 'ohos.permission.WRITE_CALENDAR'];
    let atManager: abilityAccessCtrl.AtManager = abilityAccessCtrl.createAtManager();
    let mContext: common.UIAbilityContext = GetFromGlobalThis("context") as common.UIAbilityContext;
    // 1. 请求权限
    atManager.requestPermissionsFromUser(mContext, permissions).then((result: PermissionRequestResult) => {
      console.log(`get Permission success, result: ${JSON.stringify(result)}`);
      calendarMgr = calendarManager.getCalendarManager(mContext);
      // 2. 创建日历账户
      calendarMgr?.createCalendar(CalendarManager.calendarAccount, (err: BusinessError, calendar: calendarManager.Calendar) => {
        if (err) {
          console.error(`Failed to create calendar. Code: ${err.code}, message: ${err.message}`);
          return
        }
        // 3. 创建日程
        let eventId: number | undefined = undefined;
        const date = new Date();
        const event: calendarManager.Event = {
          title: title,
          type: calendarManager.EventType.NORMAL,
          startTime: date.getTime(),
          endTime: date.getTime() + 60 * 60 * 1000,
          reminderTime: [10],
          recurrenceRule: {
            recurrenceFrequency: calendarManager.RecurrenceFrequency.DAILY,
            count: 100,
            interval: 2,
            expire: date.getTime() + 60 * 60 * 1000 * 3,
            excludedDates: [date.getTime() + 60 * 60 * 1000 * 2]
          },
          // 日程服务，可选字段，需要一键服务功能的日程，填写该属性。
          service: {
            type: calendarManager.ServiceType.TRIP,
            uri: 'http://www.baidu.com',
            description: '一键服务'
          }
        };
        calendar.addEvent(event).then((data: number) => {
          console.info(`Succeeded in adding event, id -> ${data}`);
          eventId = data;
        }).catch((err: BusinessError) => {
          console.error(`Failed to addEvent. Code: ${err.code}, message: ${err.message}`);
        });


        // 4. 设置日历配置信息，打开日程提醒、设置日历账户颜色
        calendar.setConfig(CalendarManager.config).then(() => {
          console.info(`Succeeded in setting config, data->${JSON.stringify(CalendarManager.config)}`);
        }).catch((err: BusinessError) => {
          console.error(`Failed to set config. Code: ${err.code}, message: ${err.message}`);
        });
      })

    }).catch((error: BusinessError) => {
      console.error(`get Permission error, error. Code: ${error.code}, message: ${error.message}`);
    });
    return true;
  }

  public static DeleteCalendar(title: string): number {
    // UnityAndroidPermission 138
    //     let calendarMgr: calendarManager.CalendarManager | null = this.getCalendarMgr();
    let calendarMgr: calendarManager.CalendarManager | null = null;
    const permissions: Permissions[] = ['ohos.permission.READ_CALENDAR', 'ohos.permission.WRITE_CALENDAR'];
    let atManager: abilityAccessCtrl.AtManager = abilityAccessCtrl.createAtManager();
    let mContext: common.UIAbilityContext = GetFromGlobalThis("context") as common.UIAbilityContext;
    // 1. 请求权限
    atManager.requestPermissionsFromUser(mContext, permissions).then((result: PermissionRequestResult) => {
      console.log(`get Permission success, result: ${JSON.stringify(result)}`);
      calendarMgr = calendarManager.getCalendarManager(mContext);
      // 2. 创建日历账户
      calendarMgr?.createCalendar(CalendarManager.calendarAccount, (err: BusinessError, calendar: calendarManager.Calendar) => {
        if (err) {
          console.error(`Failed to create calendar. Code: ${err.code}, message: ${err.message}`);
          return
        }
        // 3. 根据日程标题查询日程
        const filterTitle = calendarManager.EventFilter.filterByTitle('update');
        calendar.getEvents(filterTitle).then((data: calendarManager.Event[]) => {
          console.info(`Succeeded in getting events, data -> ${JSON.stringify(data)}`);
          // 4. 删除这些日程
          for (let i = 0; i < data.length; i++ ) {
            calendar.deleteEvent(data[i].id)
          }
        }).catch((err: BusinessError) => {
          console.error(`Failed to get events. Code: ${err.code}, message: ${err.message}`);
        });

      })

    }).catch((error: BusinessError) => {
      console.error(`get Permission error, error. Code: ${error.code}, message: ${error.message}`);
    });
    return 0;
  }

  public static GetCalendar(title: string): string {
    return "";
  }

  public static EditCalendar(oldTitle: string, newTitle: string, content: string, remindTime: number):boolean {
    return true
  }

  public static CheckSelfPermission(permissionString: string): boolean{
    return PermissionControl.checkSelfPermission(permissionString);
  }

  public static CheckSelfPermissionStatus(permissionString: string): number {
    return PermissionControl.checkSelfPermissionStatus(permissionString);
  }

  public static RequestSelfPermission(permissionString: string): void {
    let permissions: Array<Permissions> = [permissionString as Permissions];
    if (permissionString == 'ohos.permission.LOCATION') {
      permissions.push('ohos.permission.APPROXIMATELY_LOCATION')
    }
    PermissionControl.reqPermissionsFromUser(permissions);
  }

  public static OpenSelfPermission(permissionString: string): void {
    PermissionControl.reqPermissionOnSetting(permissionString);
  }


}


export class PermissionControl {
  static reqPermissionOnSetting(permissionStr: string): void {
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
        PermissionControl.reqPermissionsFromUser(permissions);
      }
    });
  }
  static reqPermissionsFromUser(permissions: Array<Permissions>): void {
    let atManager: abilityAccessCtrl.AtManager = abilityAccessCtrl.createAtManager();
    let context: common.UIAbilityContext = GetFromGlobalThis("context") as common.UIAbilityContext;
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
  static checkSelfPermission(permissionStr: string): boolean{
    let permission: Permissions = permissionStr as Permissions;
    let grantStatus: abilityAccessCtrl.GrantStatus = PermissionControl.checkPermissionGrant(permission);
    return grantStatus === abilityAccessCtrl.GrantStatus.PERMISSION_GRANTED;
  }
  static checkSelfPermissionStatus(permissionStr:string): number{
    // 0 拒绝    1 允许      2 拒绝且不再询问       3 没有授权，需要第一次询问。
    // 鸿蒙暂不支持查询是否第一次询问。
    if (PermissionControl.checkSelfPermission(permissionStr)) return 1;
    return 3;
  }
  static checkPermissionGrant(permission: Permissions): abilityAccessCtrl.GrantStatus {
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

  static areNotificationsEnabled():boolean {
    return notificationManager.isNotificationEnabledSync();
  }
  static requestEnableNotification():void {
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
  register["PermissionPlugin"] = CalendarManager;
  return register;
}
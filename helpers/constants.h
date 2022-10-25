#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <QNetworkAccessManager>
#endif  // CONSTANTS_H

namespace crm {

typedef enum {
  GetOffices,
  GetStudyRequests,
  GetLeads,
  GetLeadStatuses,
  GetHistoryModifyLeadStatus,
  GetStudents,
  GetCompanies,
  GetClientFiles,
  GetStudentPersonalOfficeSettings,
  GetTeachers,
  GetEdUnits,
  GetEdUnitStudents,
  GetEdUnitLeads,
  GetLessonPlans,
  GetEntranceTests,
  GetOfflineTestTypes,
  GetPersonalTestResults,
  GetEdUnitTestResults,
  GetOnlineTestResults,
  GetEdUnitStudentReports,
  GetSupplies,
  GetPrices,
  GetPayments,
  GetIncomesAndOutgoes,
  GetEdMaterials,
  GetAnnouncements,
  GetCollocutors,
  GetCorrespondence,
  GetEdUnitChats,
  GetEdUnitChatMessages,
  AddLead,
  AddStudent,
  AddStudentFromLead,
  EditPersonal,
  EditContacts,
  EditAgentContacts,
  EditIndClientParams,
  EditUserExtraFields,
  UploadPhoto,
  RemovePhoto,
  SetLeadStatus,
  SetClientStatus,
  AddEdUnitLead,
  AddEdUnitStudent,
  EditEdUnitStudent,
  SetStudentPasses,
  AddPayment,
  EditPayment,
  AddEdUnitPayment,
  AddEditPersonalTestResult,
  AddEditEdUnitTestResult,
  AddTask,
  GetMemberAuthKey,
  GetAuthMember,
  AddStudyRequest
} RequestName;

const QUrl API_ENDPOINT("https://movavischool.t8s.ru/Api/V2/");

}  // namespace crm

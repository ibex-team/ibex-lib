# interface is empty for 'none'
create_target_import_and_export (none "IGNORE" NONE_EXPORTFILE NAMESPACE Ibex::)

list (APPEND EXPORTFILES "${NONE_EXPORTFILE}")
set (EXPORTFILES "${EXPORTFILES}" PARENT_SCOPE)

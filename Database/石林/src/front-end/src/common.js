'use strict';

let CATEGORY = {
    NEWS_TYPE: 0,
    NEWS_STATUS: 1,
    FILE_TYPE: 2,
    LOCALE: 3,
};

let ROLE = {
    ADMINISTRATOR: 0,
    OFFICER: 1,
    COMMON: 2,
    DELETED: 3,
}

function categoryToString (arg) {
    switch(Number(arg)) {
    case CATEGORY.NEWS_TYPE:
        return 'NEWS_TYPE';
    case CATEGORY.NEWS_STATUS:
        return 'NEWS_STATUS';
    case CATEGORY.FILE_TYPE:
        return 'FILE_TYPE';
    case CATEGORY.LOCALE:
        return 'LOCALE';
    default:
        return 'UNKNOWN_CATEGORY';
    }
}

function roleToString(arg) {
    switch(Number(arg)) {
    case ROLE.ADMINISTRATOR:
        return 'ADMINISTRATOR';
    case ROLE.OFFICER:
        return 'OFFICER';
    case ROLE.COMMON:
        return 'COMMON';
    case ROLE.DELETED:
        return 'DELETED';
    default:
        return 'UNKNOWN_ROLE';
    }
}


export default {
    CATEGORY, categoryToString,
    ROLE, roleToString,
}

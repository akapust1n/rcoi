export function _parseJSON(response) {
    return response.text().then(function (text) {
        return text ? JSON.parse(text) : {}
    }).catch((err) => {
        return {};
    });
}
export function isEmpty(obj) {
    if (obj === undefined)
        return true;
    return Object.keys(obj).length === 0;
}
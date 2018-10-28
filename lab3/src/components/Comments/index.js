import React, { Component } from 'react'
import _parseJSON from "../../HttpAssist"
import { Panel, Badge } from "react-bootstrap"

export default class Comment extends Component {
    constructor(props) {
        super(props);

        this.state = {
            login: this.props.comment.name,
            body: this.props.comment.body,
            rating: this.props.comment.rating,
        };
        console.log(this.props.comment);
    }


    render() {
        return (
            <Panel>
                <Panel.Heading>{this.state.login}</Panel.Heading>
                <Panel.Body>{this.state.body}
                    <Badge pullRight={true}>{this.state.rating}</Badge>
                </Panel.Body>
            </Panel>
        )
    }

}


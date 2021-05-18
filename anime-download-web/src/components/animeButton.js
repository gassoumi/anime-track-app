import React, { useState } from 'react'
import ModalCreateAnime from "./modalCreateAnime";

export default function AnimeButton() {
    const [show, setShow] = useState(false);

    const handleClose = () => setShow(false);
    const handleShow = () => setShow(true);

    return (
        <div className="row">
            <div className="col-12">
                <ModalCreateAnime show={show} handleClose={handleClose} />
                <div className="d-flex justify-content-end">
                    <button onClick={handleShow} type="button" className="btn btn-success">Create new anime </button>
                </div>
            </div>
        </div>
    )
}
